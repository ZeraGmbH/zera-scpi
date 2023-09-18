#!/usr/bin/env python

from typing import List
import os
import sys
sys.path.insert(0, '.')
import argparse
import logging
from src.message_parser import MessageParser, CommandType, MessageData
from src.message_handlers import TCPHandler


class ExecScpiCmdsProgram:
    @staticmethod
    def run() -> int:
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
        try:
            args = parser.parse_args()
        except SystemExit:
            return 1

        if not os.path.exists(args.input_file):
            logging.info(f"File \"{args.input_file}\" does not exist!")
            return 2

        if (messages_and_lines := MessageParser.read_messages_with_lines_from_file(args.input_file)) is None:
            logging.info(f"Reading file \"{args.input_file}\" failed!")
            return 3

        messages: List[MessageData] = [MessageParser.get_message_data_from_string(*message_and_line, len(messages_and_lines[0]))
                                       for message_and_line in zip(*messages_and_lines)]

        logging.info("Checking messages...")
        invalid_message_found = False
        for message in messages:
            if not message.is_valid:
                for command in message.commands:
                    if command.command_type is CommandType.UNKNOWN:
                        logging.info(f" > Invalid command found in line {message.file_line_number}:{command.position_in_message}")
                    elif command.command_type is CommandType.EMPTY:
                        logging.info(f" > Empty command found in line {message.file_line_number}:{command.position_in_message}")
                    # TODO Check for ASCII characters and allowed symboles only: a-zA-z0-9.:,;*?-+[[:blank:]] (even more symbols?)
                invalid_message_found = True
        if not invalid_message_found:
            logging.info("... all messages are valid.")
        else:
            logging.info("... invalid messages(s) found!")

        logging.info(f"Connecting to {args.ip_address}:{args.port_number}...")
        tcp_handler = TCPHandler(args.ip_address, args.port_number, args.receive_timeout)
        if tcp_handler.connected:
            logging.info(f"... successfully connected to {args.ip_address}:{args.port_number}.")
        else:
            logging.info(f"... establishing connection failed!")
            return 4

        if len(messages) > 0:
            logging.info("Sending messages...")
            for message in messages:
                line_number_string = str(message.file_line_number).zfill(message.command_count_string_width)
                indices_of_expected_responses = [idx for idx, command in enumerate(message.commands) if command.command_type is CommandType.QUERY]
                expected_responses_count_string_width = len(str(len(indices_of_expected_responses)))
                logging.info(f"==> [L{line_number_string}] {message.original_message}")
                message_part_position = ""
                for c, command in enumerate(message.commands):
                    message_part_position = message_part_position.ljust(command.position_in_message, " ")
                    message_part_position += f"[{str(c).zfill(expected_responses_count_string_width)}]"
                logging.info(f"{''.ljust(len(line_number_string) + 8)}{message_part_position}")
                tcp_handler.send_message(message.original_message + "\n")
                for r in range(len(indices_of_expected_responses)):
                    response = tcp_handler.receive_response()
                    if response is not None:
                        logging.info(f" <-[{str(indices_of_expected_responses[r]).zfill(expected_responses_count_string_width)}] {response}")
                    else:
                        logging.info("Timeout on receiving response.")
        else:
            logging.info("No messages to send.")

        logging.info(f"Disconnecting from {args.ip_address}:{args.port_number}...")
        del tcp_handler

        return 0


def main() -> None:
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="[%(asctime)s.%(msecs)03d]: %(message)s", datefmt="%H:%M:%S")
    program = ExecScpiCmdsProgram
    result = program.run()
    exit(result)


if __name__ == "__main__":
    main()
