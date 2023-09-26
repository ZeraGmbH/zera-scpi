import logging
import sys
sys.path.insert(0, '.')
import unittest
from src.message_handlers import TCPHandler
from testlib.tcp_server_helper import VerboseTcpServer, EchoVerboseTcpServer, PortNumberGenerator


def setUpModule():  # pylint: disable=invalid-name
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    start_port = 16320
    port_cnt = 100
    PortNumberGenerator.set_port_range(start_port, start_port + port_cnt - 1)


class TestTCPHandler(unittest.TestCase):
    def test_connect(self) -> None:
        ip_address = "localhost"
        port_number = PortNumberGenerator.get_next_port_number()  # use port different from proper one to avoid conflicts
        server = VerboseTcpServer(ip_address, port_number)
        server.run(run_in_background=True)
        tcp_handler = TCPHandler(ip_address, port_number, receive_timeout=1000)
        try:  # Make sure the TCP handler gets disconnected on assertion exception
            self.assertTrue(tcp_handler.connected)
        except Exception as exception:
            raise exception  # Propagate exception to unittest
        finally:
            del tcp_handler
            server.quit()

    def test_send_receive(self) -> None:
        ip_address = "localhost"
        port_number = PortNumberGenerator.get_next_port_number()  # Use different port for each test to allow them being executed in parallel
        server = EchoVerboseTcpServer(ip_address, port_number)
        server.run(run_in_background=True)
        tcp_handler = TCPHandler(ip_address, port_number, receive_timeout=1000)
        message = " test_string \n"
        tcp_handler.send_message(message)
        response = tcp_handler.receive_response()
        try:  # Make sure the TCP handler gets disconnected on assertion exception
            self.assertEqual(message.rstrip('\n'), response)
        except Exception as exception:
            raise exception  # Propagate exception to unittest
        finally:
            del tcp_handler
            server.quit()
