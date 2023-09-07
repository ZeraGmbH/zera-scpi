#!/usr/bin/env python

import os
import sys
sys.path.insert(0, '.')
import argparse
from src.message_parser import MessageParser
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
            print(f"File \"{args.input_file}\" does not exist!", file=sys.stderr)
            exit(1)

        if (messages := MessageParser.read_messages_from_file(args.input_file)) is None:
            print(f"Reading file \"{args.input_file}\" failed!", file=sys.stderr)
            exit(2)

        print(f"Connect to {args.ip_address}:{args.port_number}...")
        tcp_handler = TCPHandler(args.ip_address, args.port_number, args.receive_timeout)
        if tcp_handler.connected:
            print(f"Successfully connected to {args.ip_address}:{args.port_number}.")
        else:
            print(f"Establishing connection failed!", file=sys.stderr)
            exit(3)

        print("Sending messages...")
        for message in messages:
            print(f"{message}")
            tcp_handler.send_message(message + "\n")

            number_of_expected_responses = message.count("?")
            for r in range(number_of_expected_responses):
                response = tcp_handler.receive_response()
                if response is not None:
                    print(f"> {response}")
                else:
                    print("Timeout on receiving response.")


def main():
    program = ExecScpiCmdsProgram
    program.run()


if __name__ == "__main__":
    main()
