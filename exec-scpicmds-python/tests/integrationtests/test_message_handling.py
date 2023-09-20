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
    start_port = 16420
    port_cnt = 100
    PortNumberGenerator.set_port_range(start_port, start_port + port_cnt - 1)


class TestMessageHandling(unittest.TestCase):
    def test_read_message_file_and_send_receive_over_tcpip(self) -> None:
        # Prepare and read message file
        filename = FileHelper.get_filename_from_class_and_method(self, self.test_read_message_file_and_send_receive_over_tcpip) + ".txt"
        filename = os.path.join(os.path.dirname(os.path.abspath(__file__)), filename)
        lines = []
        lines.append("*IDN?")
        lines.append("*STB?")
        lines.append("MEASURE:FFT1:UL1?")
        lines.append("TEST;")
        messages = None
        try:
            with FileWriterHelper(filename, lines):
                try:
                    messages = MessageParser.read_messages_from_file(filename)
                except OSError as exception:
                    self.fail(f"Reading temporary test file \"{filename}\" failed with exception \"{exception}\".")
        except OSError as exception:
            self.fail(f"Writing temporary test file \"{filename}\" failed with exception \"{exception}\".")
        # Send messages over TCP/IP and read echo
        ip_address = "localhost"
        port_number = PortNumberGenerator.get_next_port_number()  # Use different port for each test to allow them being executed in parallel
        server = EchoVerboseTcpServer(ip_address, port_number)
        server.run(run_in_background=True)
        tcp_handler = TCPHandler(ip_address, port_number, receive_timeout=1000)
        try:  # Make sure the TCP handler gets disconnected on assertion exception
            for message_idx, message in enumerate(lines):
                tcp_handler.send_message(message + "\n")
                response = tcp_handler.receive_response()
                self.assertEqual(message, response, f"Message #{message_idx} \"{message}\" is not received as it was sent.")
        except Exception as exception:
            raise exception  # Propagate exception to unittest
        finally:
            del tcp_handler
            server.quit()


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    unittest.main()
