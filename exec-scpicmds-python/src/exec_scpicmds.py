#!/usr/bin/env python

from typing import List
import os
import sys
sys.path.insert(0, '.')
import argparse
import logging
from src.logging_handler import Logging, LoggingColor, LoggingStyle
from src.message_parser import MessageParser, CommandType, MessageData
from src.message_handlers import TCPHandler


class ExecScpiCmdsProgram:
    @staticmethod
    def run() -> int:
        Logging.setup(log_level=logging.DEBUG)

        def check_positive_integer(value: str, excl_zero: bool = False) -> int:
            int_value = int(value)
            if (excl_zero and int_value <= 0) or (not excl_zero and int_value < 0):
                raise argparse.ArgumentTypeError(f"{value} is not a positive int value!")
            return int_value

        def check_port_number(value: str) -> int:
            int_value = int(value)
            if not 0 < int_value <= 65535:
                raise argparse.ArgumentTypeError(f"{value} is not a valid port number!")
            return int_value
    
        parser = argparse.ArgumentParser(description="Execute SCPI commands and optionally check results.")
        parser.add_argument("-f", "--input-file", required=True,
                            help="Path to file containing SCPI commands.")
        parser.add_argument("-i", "--ip-address", required=True,
                            help="IP-address of instrument.")
        parser.add_argument("-p", "--port-number", required=True, type=check_port_number,
                            help="Port number of instrument.")
        parser.add_argument("-t", "--receive-timeout", type=lambda x: check_positive_integer(x, excl_zero=True), default=3000,
                            help="Receive timeout [ms] of TCP/IP-connection to instrument.")
        parser.add_argument("--enable-formatted-output", action="store_true", default=False,
                            help="Enable output formatted with colors and styles.")
        parser.add_argument("-r", "--number-of-repetitions", type=lambda x: check_positive_integer(x, excl_zero=False), default=1,
                            help="Number of repetitions sending whole SCPI command file.")
        parser.add_argument("--sync_cmds_with_opc", action="store_true", default=False,
                            help="Add an \"|*OPC?\" query after each command to synchronize it. "\
                                "Also expects more responses, which will be hidden from the user. Only that way a timeout can get detected. "\
                                "\033[0;31mAttention:\033[0m this feature only makes sense when queueing of SCPI commands on the target instrument is activated.")

        try:
            args = parser.parse_args()
        except SystemExit:
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

        Logging.log_debug_msg(f"Connecting to {args.ip_address}:{args.port_number}...")
        tcp_handler = TCPHandler(args.ip_address, args.port_number, args.receive_timeout)
        if tcp_handler.connected:
            Logging.log_debug_msg(f"... successfully connected to {args.ip_address}:{args.port_number}.", LoggingColor.GREEN)
        else:
            Logging.log_debug_msg(f"... establishing connection failed!", LoggingColor.RED)
            return 4

        if len(messages) > 0:
            for current_repetition in range(args.number_of_repetitions):
                ExecScpiCmdsProgram._print_sending_messages(current_repetition, args.number_of_repetitions)
                for message in messages:
                    message_part_indices_string_width = len(str(len(message.commands) + 1))
                    indices_of_expected_responses = [idx for idx, command in enumerate(message.commands) if command.command_type is CommandType.QUERY]
                    expected_responses_max_idx_string_width = len(str(max(indices_of_expected_responses) + 1)) if len(indices_of_expected_responses) > 0 else 0
                    ExecScpiCmdsProgram._print_message_with_part_indices(message, message_part_indices_string_width)
                    if not args.sync_cmds_with_opc:
                        ExecScpiCmdsProgram._send_message_and_read_response(tcp_handler, message, indices_of_expected_responses, expected_responses_max_idx_string_width)
                    else:
                        ExecScpiCmdsProgram._send_message_and_read_response_with_opc(tcp_handler, message, expected_responses_max_idx_string_width)
        else:
            Logging.log_debug_msg("No messages to send.", LoggingColor.BLUE)

        Logging.log_debug_msg(f"Disconnecting from {args.ip_address}:{args.port_number}...")
        del tcp_handler

        return 0

    @staticmethod
    def _send_message_and_read_response(tcp_handler: TCPHandler, message: str, indices_of_expected_responses: List[int], expected_responses_max_idx_string_width: int) -> None:
        message_string = message.original_message
        tcp_handler.send_message(message_string + "\n")
        for r in range(len(indices_of_expected_responses)):
            response = tcp_handler.receive_response()
            if response is not None:
                Logging.log_debug_msg(f" <-[{str(indices_of_expected_responses[r] + 1).zfill(expected_responses_max_idx_string_width)}] {response}")
            else:
                Logging.log_debug_msg(f" <-[{str(indices_of_expected_responses[r] + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on receiving response.", LoggingColor.RED)

    @staticmethod
    def _send_message_and_read_response_with_opc(tcp_handler: TCPHandler, message: str, expected_responses_max_idx_string_width: int) -> None:
        message_string = "|".join([cmd.command if cmd.command_type is CommandType.QUERY else cmd.command + "|" + "*OPC?" for cmd in message.commands])
        tcp_handler.send_message(message_string + "\n")
        for r in range(len(message.commands)):
            response = tcp_handler.receive_response()
            if response is not None:
                if message.commands[r].command_type is CommandType.QUERY:
                    Logging.log_debug_msg(f" <-[{str(r + 1).zfill(expected_responses_max_idx_string_width)}] {response}")
            else:
                if message.commands[r].command_type is CommandType.QUERY:
                    Logging.log_debug_msg(f" <-[{str(r + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on receiving response.", LoggingColor.RED)
                else:
                    Logging.log_debug_msg(f" <-[{str(r + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on executing command.", LoggingColor.RED)

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
        for c, command in enumerate(message.commands):
            message_part_position = message_part_position.ljust(command.position_in_message, " ")
            message_part_position += f"[{str(c + 1).zfill(expected_responses_count_string_width)}]"
        Logging.log_debug_msg(f"{''.ljust(len(line_number_string) + 8)}{message_part_position}", LoggingColor.GREEN)


def main() -> None:
    program = ExecScpiCmdsProgram
    result = program.run()
    exit(result)


if __name__ == "__main__":
    main()
