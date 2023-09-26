from typing import List, Tuple
import logging
import os
import sys
sys.path.insert(0, '.')
import unittest
from src.message_parser import MessageParser
from src.message_handlers import TCPHandler
from testlib.file_helper import FileHelper
from testlib.message_parser_helper import FileWriterHelper
from testlib.tcp_server_helper import EchoVerboseTcpServer, PortNumberGenerator


def setUpModule():  # pylint: disable=invalid-name
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    start_port = 16420
    port_cnt = 100
    PortNumberGenerator.set_port_range(start_port, start_port + port_cnt - 1)


# TODO fix logic: variable "messages" is not used after setting it, but most likely should be used to complete the whole test cycle: file -> send -> receive -> compare
# TODO split function into smaller parts: make two helper functions e.g. called prepare_and_read_message_file() and send_messages__and_read_echo() and remove the corresponding comments above each of these blocks
class TestMessageHandling(unittest.TestCase):
    def _prepare_and_read_message_file(self) -> Tuple[List[str], List[str]]:
        filename = FileHelper.get_filename_from_class_and_method(self, self.test_read_message_file_and_send_receive_over_tcpip) + ".txt"
        filename = os.path.join(os.path.dirname(os.path.abspath(__file__)), filename)
        lines = []
        lines.append("*IDN?")
        lines.append("*STB?")
        lines.append("MEASURE:FFT1:UL1?")
        lines.append("TEST;")
        messages = []
        try:
            with FileWriterHelper(filename, lines):
                try:
                    messages = MessageParser.read_messages_from_file(filename)
                except OSError as exception:
                    self.fail(f"Reading temporary test file \"{filename}\" failed with exception \"{exception}\".")
        except OSError as exception:
            self.fail(f"Writing temporary test file \"{filename}\" failed with exception \"{exception}\".")
        return lines, messages

    def _send_messages_and_read_echo(self, lines: List[str], messages: List[str]) -> None:
        ip_address = "localhost"
        port_number = PortNumberGenerator.get_next_port_number()  # Use different port for each test to allow them being executed in parallel
        server = EchoVerboseTcpServer(ip_address, port_number)
        server.run(run_in_background=True)
        tcp_handler = TCPHandler(ip_address, port_number, receive_timeout=1000)
        try:  # Make sure the TCP handler gets disconnected on assertion exception
            for message_idx, message in enumerate(messages):
                tcp_handler.send_message(message + "\n")
                response = tcp_handler.receive_response()
                self.assertEqual(lines[message_idx], response, f"Message #{message_idx} \"{message}\" is not received as it was sent.")
        except Exception as exception:
            raise exception  # Propagate exception to unittest
        finally:
            del tcp_handler
            server.quit()

    def test_read_message_file_and_send_receive_over_tcpip(self) -> None:
        lines, messages = self._prepare_and_read_message_file()
        self._send_messages_and_read_echo(lines, messages)
