import logging
import os
import sys
sys.path.insert(0, '.')
import unittest
import socket
from src.message_parser import MessageParser
from src.message_handlers import TCPHandler
from testlib.message_parser_helper import FileWriterHelper
from testlib.tcp_server_helper import VerboseTcpServer


class EchoVerboseTcpServer(VerboseTcpServer):
    def __init__(self, ip_address: str, port_number: int) -> None:
        VerboseTcpServer.__init__(self, ip_address, port_number)

    def on_client_receive_message(self, client: socket.socket, message: str) -> None:
        super().on_client_receive_message(client, message)
        client.send(message)  # Echo message back to client


class TestMessageHandling(unittest.TestCase):
    def test_read_message_file_and_send_receive_over_tcpip(self) -> None:
        # Prepare and read message file
        filename = os.path.join(os.path.dirname(os.path.abspath(__file__)), "example_file_for_test_read_message_file_and_send_receive_over_tcpip.txt")
        lines = list()
        lines.append("*IDN?")
        lines.append("*STB?")
        lines.append("MEASURE:FFT1:UL1?")
        lines.append("TEST;")
        messages = None
        try:
            with FileWriterHelper(filename, lines):
                try:
                    messages = MessageParser.read_messages_from_file(filename)
                except Exception as e:
                    self.assertTrue(False, f"Reading temporary test file \"{filename}\" failed with exception \"{e}\".")
        except Exception as e:
            self.assertTrue(False, f"Writing temporary test file \"{filename}\" failed with exception \"{e}\".")
        # Send messages over TCP/IP and read echo
        ip_address = "localhost"
        port_number = 16322  # Use different port for each test to allow them being executed in parallel
        server = EchoVerboseTcpServer(ip_address, port_number)
        server.run(run_in_background=True)
        tcp_handler = TCPHandler(ip_address, port_number, receive_timeout=1000)
        try:  # Make sure the TCP handler gets disconnected on assertion exception
            for m, message in enumerate(lines):
                tcp_handler.send_message(message + "\n")
                response = tcp_handler.receive_response()
                self.assertEqual(message, response, f"Message #{m} \"{message}\" is not received as it was sent.")
        except Exception as e:
            raise e  # Propagate exception to unittest
        finally:
            del tcp_handler
            server.quit()


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    unittest.main()
