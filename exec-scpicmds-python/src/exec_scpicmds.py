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
    def run() -> None:
        parser = argparse.ArgumentParser(description="Execute SCPI commands and optionally check results.")
        parser.add_argument("-f", "--input-file", required=True,
                            help="Path to file containing SCPI commands.")
        parser.add_argument("-i", "--ip-address", required=True,
                            help="IP-address of instrument.")
        parser.add_argument("-p", "--port-number", required=True, type=int,
                            help="Port number of instrument.")
        parser.add_argument("-t", "--receive-timeout", type=int, default=3000,
                            help="Receive timeout [ms] of TCP/IP-connection to instrument.")
        args = parser.parse_args()

        if not os.path.exists(args.input_file):
            logging.info(f"File \"{args.input_file}\" does not exist!", file=sys.stderr)
            exit(1)

        if (messages_and_lines := MessageParser.read_messages_with_lines_from_file(args.input_file)) is None:
            logging.info(f"Reading file \"{args.input_file}\" failed!", file=sys.stderr)
            exit(2)
        
        messages: List[MessageData] = [MessageParser.get_message_data_from_string(*message_and_line) for message_and_line in zip(*messages_and_lines)]

        logging.info(f"Connect to {args.ip_address}:{args.port_number}...")
        tcp_handler = TCPHandler(args.ip_address, args.port_number, args.receive_timeout)
        if tcp_handler.connected:
            logging.info(f"Successfully connected to {args.ip_address}:{args.port_number}.")
        else:
            logging.info(f"Establishing connection failed!", file=sys.stderr)
            exit(3)

        logging.info("Sending messages...")
        for message in messages:
            logging.info(f"{message.original_message}")
            tcp_handler.send_message(message.original_message + "\n")
            number_of_expected_responses = len([command for command in message.commands if command.command_type is CommandType.QUERY])
            for r in range(number_of_expected_responses):
                response = tcp_handler.receive_response()
                if response is not None:
                    logging.info(f"> {response}")
                else:
                    logging.info("Timeout on receiving response.")


def main():
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(asctime)s.%(msecs)03d: %(message)s", datefmt="%H:%M:%S")
    program = ExecScpiCmdsProgram
    program.run()


if __name__ == "__main__":
    main()
