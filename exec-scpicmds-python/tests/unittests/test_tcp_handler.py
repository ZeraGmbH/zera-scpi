import logging
import sys
sys.path.insert(0, '.')
import unittest
import socket
from src.message_handlers import TCPHandler
from testlib.tcp_server_helper import VerboseTcpServer


class EchoVerboseTcpServer(VerboseTcpServer):
    def __init__(self, ip_address: str, port_number: int) -> None:
        VerboseTcpServer.__init__(self, ip_address, port_number)

    def on_client_receive_message(self, client: socket.socket, message: str) -> None:
        super().on_client_receive_message(client, message)
        client.send(message)  # Echo message back to client


class TestTCPHandler(unittest.TestCase):
    def test_connect(self) -> None:
        ip_address = "localhost"
        port_number = 16320  # use port different from proper one to avoid conflicts
        server = VerboseTcpServer(ip_address, port_number)
        server.run(run_in_background=True)
        tcp_handler = TCPHandler(ip_address, port_number, receive_timeout=1000)
        try:  # Make sure the TCP handler gets disconnected on assertion exception
            self.assertTrue(tcp_handler.connected)
        except Exception as e:
            raise e  # Propagate exception to unittest
        finally:
            del tcp_handler
            server.quit()

    def test_send_receive(self) -> None:
        ip_address = "localhost"
        port_number = 16321  # Use different port for each test to allow them being executed in parallel
        server = EchoVerboseTcpServer(ip_address, port_number)
        server.run(run_in_background=True)
        tcp_handler = TCPHandler(ip_address, port_number, receive_timeout=1000)
        message = " test_string \n"
        tcp_handler.send_message(message)
        response = tcp_handler.receive_response()
        try:  # Make sure the TCP handler gets disconnected on assertion exception
            self.assertEqual(message.rstrip('\n'), response)
        except Exception as e:
            raise e  # Propagate exception to unittest
        finally:
            del tcp_handler
            server.quit()


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    unittest.main()