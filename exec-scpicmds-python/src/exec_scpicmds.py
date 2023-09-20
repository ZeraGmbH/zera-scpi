#!/usr/bin/env python

from typing import List, Optional
import os
import sys
sys.path.insert(0, '.')
import argparse
import logging
import re
from src.logging_handler import Logging, LoggingColor, LoggingStyle
from src.message_parser import MessageParser, CommandType, MessageData
from src.message_handlers import TCPHandler


class ExecScpiCmdsArgsParser:
    @staticmethod
    def _check_positive_integer(value: str, excl_zero: bool=False) -> int:
        int_value = int(value)
        if (excl_zero and int_value <= 0) or (not excl_zero and int_value < 0):
            raise argparse.ArgumentTypeError(f"{value} is not a positive int value!")
        return int_value

    @staticmethod
    def _check_port_number(value: str) -> int:
        int_value = int(value)
        if not 0 < int_value <= 65535:
            raise argparse.ArgumentTypeError(f"{value} is not a valid port number!")
        return int_value

    @staticmethod
    def _check_ip_address_or_hostname(value: str) -> str:
        ip_pattern = r"^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$"
        hostname_pattern = r"^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])$"
        if not (re.match(ip_pattern, value) or re.match(hostname_pattern, value)):
            raise argparse.ArgumentTypeError(f"{value} is not a valid ip address or hostname!")
        return value

    @staticmethod
    def parse(args: Optional[List[str]]=None) -> Optional[argparse.Namespace]:
        parser = argparse.ArgumentParser(description="Execute SCPI commands and optionally check results.")
        parser.add_argument("-f", "--input-file", required=True,
                            help="Path to file containing SCPI commands.")
        parser.add_argument("-i", "--ip-address", required=True, type=ExecScpiCmdsArgsParser._check_ip_address_or_hostname,
                            help="IP-address of instrument.")
        parser.add_argument("-p", "--port-number", required=True, type=ExecScpiCmdsArgsParser._check_port_number,
                            help="Port number of instrument.")
        parser.add_argument("-t", "--receive-timeout", type=lambda x: ExecScpiCmdsArgsParser._check_positive_integer(x, excl_zero=True), default=3000,
                            help="Receive timeout [ms] of TCP/IP-connection to instrument.")
        parser.add_argument("--enable-formatted-output", action="store_true", default=False,
                            help="Enable output formatted with colors and styles.")
        parser.add_argument("-r", "--number-of-repetitions", type=lambda x: ExecScpiCmdsArgsParser._check_positive_integer(x, excl_zero=False), default=1,
                            help="Number of repetitions sending whole SCPI command file.")
        parser.add_argument("--sync_cmds_with_opc", action="store_true", default=False,
                            help="Add an \"|*OPC?\" query after each command to synchronize it. "\
                                "Also expects more responses, which will be hidden from the user. Only that way a timeout can get detected. "\
                                "\033[0;31mAttention:\033[0m this feature only makes sense when queueing of SCPI commands on the target instrument is activated.")
        try:
            if args is None:
                parser_args = parser.parse_args()
            else:
                parser_args = parser.parse_args(args)
            return parser_args
        except SystemExit:
            return None


class ExecScpiCmdsProgram:
    @staticmethod
    def run() -> int:
        Logging.setup(log_level=logging.DEBUG)
        args = ExecScpiCmdsArgsParser.parse()
        if args is None:
            return 1

        Logging.enable_formatted_output(args.enable_formatted_output)
        if not os.path.exists(args.input_file):
            Logging.log_debug_msg(f"File \"{args.input_file}\" does not exist!", LoggingColor.RED)
            return 2

        if (messages_and_lines := MessageParser.read_messages_with_lines_from_file(args.input_file)) is None:
            Logging.log_debug_msg(f"Reading file \"{args.input_file}\" failed!", LoggingColor.RED)
            return 3

        messages: List[MessageData] = [MessageParser.get_message_data_from_string(*message_and_line, len(messages_and_lines[0]))
                                       for message_and_line in zip(*messages_and_lines)]

        ExecScpiCmdsProgram._check_messages(messages)

        Logging.log_debug_msg(f"Connecting to {args.ip_address}:{args.port_number}...")
        tcp_handler = TCPHandler(args.ip_address, args.port_number, args.receive_timeout)
        if tcp_handler.connected:
            Logging.log_debug_msg(f"... successfully connected to {args.ip_address}:{args.port_number}.", LoggingColor.GREEN)
        else:
            Logging.log_debug_msg("... establishing connection failed!", LoggingColor.RED)
            return 4

        ExecScpiCmdsProgram._print_and_send_messages_and_read_responses(tcp_handler, messages, args.number_of_repetitions, args.sync_cmds_with_opc)

        Logging.log_debug_msg(f"Disconnecting from {args.ip_address}:{args.port_number}...")
        del tcp_handler

        return 0

    @staticmethod
    def _check_messages(messages: List[MessageData]) -> None:
        Logging.log_debug_msg("Checking messages...")
        invalid_message_found = False
        for message in messages:
            if not message.is_valid:
                for command in message.commands:
                    if command.command_type is CommandType.UNKNOWN:
                        Logging.log_debug_msg(f" > Invalid command found in line {message.file_line_number + 1}:{command.position_in_message + 1}", LoggingColor.YELLOW)
                    elif command.command_type is CommandType.EMPTY:
                        Logging.log_debug_msg(f" > Empty command found in line {message.file_line_number + 1}:{command.position_in_message + 1}", LoggingColor.YELLOW)
                    # TODO Check for ASCII characters and allowed symbols only: a-zA-z0-9.:,;*?-+[[:blank:]] (even more symbols?)
                invalid_message_found = True
        if not invalid_message_found:
            Logging.log_debug_msg("... all messages are valid.", LoggingColor.GREEN)
        else:
            Logging.log_debug_msg("... invalid messages(s) found!", LoggingColor.RED)

    @staticmethod
    def _send_message_and_read_response(tcp_handler: TCPHandler, message: str, indices_of_expected_responses: List[int], expected_responses_max_idx_string_width: int) -> None:
        message_string = message.original_message
        tcp_handler.send_message(message_string + "\n")
        for resp_idx, _ in enumerate(indices_of_expected_responses):
            response = tcp_handler.receive_response()
            if response is not None:
                Logging.log_debug_msg(f" <-[{str(indices_of_expected_responses[resp_idx] + 1).zfill(expected_responses_max_idx_string_width)}] {response}")
            else:
                Logging.log_debug_msg(f" <-[{str(indices_of_expected_responses[resp_idx] + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on receiving response.", LoggingColor.RED)

    @staticmethod
    def _send_message_and_read_response_with_opc(tcp_handler: TCPHandler, message: str, expected_responses_max_idx_string_width: int) -> None:
        message_string = "|".join([cmd.command if cmd.command_type is CommandType.QUERY else cmd.command + "|" + "*OPC?" for cmd in message.commands])
        tcp_handler.send_message(message_string + "\n")
        for resp_idx, _ in enumerate(message.commands):
            response = tcp_handler.receive_response()
            if response is not None:
                if message.commands[resp_idx].command_type is CommandType.QUERY:
                    Logging.log_debug_msg(f" <-[{str(resp_idx + 1).zfill(expected_responses_max_idx_string_width)}] {response}")
            else:
                if message.commands[resp_idx].command_type is CommandType.QUERY:
                    Logging.log_debug_msg(f" <-[{str(resp_idx + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on receiving response.", LoggingColor.RED)
                else:
                    Logging.log_debug_msg(f" <-[{str(resp_idx + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on executing command.", LoggingColor.RED)

    @staticmethod
    def _print_and_send_messages_and_read_responses(tcp_handler: TCPHandler, messages: List[str], number_of_repetitions: int, sync_cmds_with_opc: bool) -> None:
        if len(messages) > 0:
            for current_repetition in range(number_of_repetitions):
                ExecScpiCmdsProgram._print_sending_messages(current_repetition, number_of_repetitions)
                for message in messages:
                    message_part_indices_string_width = len(str(len(message.commands) + 1))
                    indices_of_expected_responses = [idx for idx, command in enumerate(message.commands) if command.command_type is CommandType.QUERY]
                    expected_responses_max_idx_string_width = len(str(max(indices_of_expected_responses) + 1)) if len(indices_of_expected_responses) > 0 else 0
                    ExecScpiCmdsProgram._print_message_with_part_indices(message, message_part_indices_string_width)
                    if not sync_cmds_with_opc:
                        ExecScpiCmdsProgram._send_message_and_read_response(tcp_handler, message, indices_of_expected_responses, expected_responses_max_idx_string_width)
                    else:
                        ExecScpiCmdsProgram._send_message_and_read_response_with_opc(tcp_handler, message, expected_responses_max_idx_string_width)
        else:
            Logging.log_debug_msg("No messages to send.", LoggingColor.BLUE)

    @staticmethod
    def _print_sending_messages(current_repetition: int, number_of_repetitions: int) -> None:
        if number_of_repetitions == 1:
            Logging.log_debug_msg("Sending messages...", LoggingColor.BLUE)
        else:
            Logging.log_debug_msg(f"Sending messages (#{current_repetition + 1}/{number_of_repetitions})...", LoggingColor.BLUE)

    @staticmethod
    def _print_message_with_part_indices(message: str, expected_responses_count_string_width: int) -> None:
        line_number_string = str(message.file_line_number + 1).zfill(message.command_count_string_width)
        Logging.log_debug_msg(f"==> [L{line_number_string}] {message.original_message}", style=LoggingStyle.BOLD)
        message_part_position = ""
        for cmd_idx, command in enumerate(message.commands):
            message_part_position = message_part_position.ljust(command.position_in_message, " ")
            message_part_position += f"[{str(cmd_idx + 1).zfill(expected_responses_count_string_width)}]"
        Logging.log_debug_msg(f"{''.ljust(len(line_number_string) + 8)}{message_part_position}", LoggingColor.GREEN)


def main() -> None:
    program = ExecScpiCmdsProgram
    result = program.run()
    sys.exit(result)


if __name__ == "__main__":
    main()
