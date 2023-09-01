#!/usr/bin/env python

from typing import Optional
from abc import ABC, abstractmethod
import os
import sys
import argparse
import socket


class IMessageHandler(ABC):
    @abstractmethod
    def send_message(self, message: str) -> None:
        pass

    @abstractmethod
    def receive_response(self) -> Optional[str]:
        pass


class TCPHandler(IMessageHandler):
    def __init__(self, ip_address: str, port_number: int, receive_timeout: Optional[int] = None) -> None:
        self._ip_address = ip_address
        self._port_number = port_number
        self._receive_timeout = receive_timeout
        self._receive_buffer = ""
        self._responses = list()
        self._connect()
    
    def __del__(self) -> None:
        if self.connected:
            self._disconnect()

    @property
    def connected(self) -> bool:
        return self._socket is not None

    def send_message(self, message: str) -> None:
        if self._socket is not None:
            self._socket.send(message.encode())
    
    def receive_response(self) -> Optional[str]:
        if len(self._responses) == 0:
            try:
                chunk = self._socket.recv(2048).decode()
            except TimeoutError:
                return None
            self._receive_buffer += chunk
            # Split up complete responses and add them to the result list
            # There might also be some results from further queries of the current message
            while "\n" in self._receive_buffer:
                idx = self._receive_buffer.index("\n")
                answer = self._receive_buffer[:idx]
                self._responses.append(answer)
                self._receive_buffer = self._receive_buffer[idx + 1:]
        return self._responses.pop(0) if len(self._responses) > 0 else None
    
    def _connect(self) -> None:
        self._socket = socket.socket()
        if self._receive_timeout is not None and self._receive_timeout >= 0:
            self._socket.settimeout(self._receive_timeout / 1000)
        try:
            self._socket.connect((self._ip_address, self._port_number))
        except:
            self._socket = None
        
    def _disconnect(self) -> None:
        self._socket.close()


class MessageParser:
    @staticmethod
    def read_messages_from_file(file_path: str) -> Optional[list[str]]:
        try:
            with open(file_path, "r") as file:
                lines = list()
                while line := file.readline():
                    line_stipped = line.strip()
                    if len(line_stipped) > 0 and line_stipped[0] != '#':
                        lines.append(line.replace("\r", "").replace("\n", ""))
                return lines
        except OSError:
            return None


class ExecScpiCmdsProgram:
    @staticmethod
    def run() -> None:
        parser = argparse.ArgumentParser(description="Execute SCPI commands and optionally check results.")
        parser.add_argument("-f", "--input-file", required=True,
                            help="Path to CMD_FILE containing SCPI commands.")
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
            exit(1)

        print(f"Connect to {args.ip_address}:{args.port_number}...")
        tcp_handler = TCPHandler(args.ip_address, args.port_number, args.receive_timeout)
        if tcp_handler.connected:
            print(f"Successfully connected to {args.ip_address}:{args.port_number}.")
        else:
            print(f"Establishing connection failed!", file=sys.stderr)
            exit(1)

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
