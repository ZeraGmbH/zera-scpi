import logging
import sys
sys.path.insert(0, '.')
import unittest
from testlib.tcp_server_helper import PortNumberGenerator


# We need a separate file as in the proper test file the PortNumberGenerator is part of the module set up
class TestPortNumberGenerator(unittest.TestCase):
    def test_set_port_range(self) -> None:
         for port_range in [[0, 100], [500, 65536], [-1, 2000], [50, 30]]:
            self.assertFalse(PortNumberGenerator.set_port_range(*port_range), f"Setting a port range {port_range} is not allowed.")
         for port_range in [[1, 65535], [100, 100]]:
            self.assertTrue(PortNumberGenerator.set_port_range(*port_range), f"Setting a port range {port_range} is allowed.")

    def test_get_next_port_number(self) -> None:
         PortNumberGenerator.set_port_range(100, 102)
         self.assertEqual(PortNumberGenerator.get_next_port_number(), 100, f"Valid next port number.")
         self.assertEqual(PortNumberGenerator.get_next_port_number(), 101, f"Valid next port number.")
         self.assertEqual(PortNumberGenerator.get_next_port_number(), 102, f"Valid next port number.")
         self.assertIsNone(PortNumberGenerator.get_next_port_number(), f"Invalid next port number.")
         PortNumberGenerator.set_port_range(1000, 1000)
         self.assertEqual(PortNumberGenerator.get_next_port_number(), 1000, f"Valid next port number.")
         self.assertIsNone(PortNumberGenerator.get_next_port_number(), f"Invalid next port number.")

    def test_get_current_port_number(self) -> None:
         PortNumberGenerator.set_port_range(4711, 4722)
         self.assertEqual(PortNumberGenerator.get_current_port_number(), 4711, f"Valid current port number.")
         self.assertEqual(PortNumberGenerator.get_current_port_number(), 4711, f"Valid current port number.")
         PortNumberGenerator.get_next_port_number()
         self.assertEqual(PortNumberGenerator.get_next_port_number(), 4712, f"Valid next port number.")


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    unittest.main()