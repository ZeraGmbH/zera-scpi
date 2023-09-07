import logging
import os
import sys
sys.path.insert(0, '.')
import unittest
from src.message_parser import MessageParser
from testlib.message_parser_helper import FileWriterHelper
from testlib.tcp_server_helper import PortNumberGenerator
from testlib.scpi_instrument_stub import ScpiInstrumentStub
from src.exec_scpicmds import ExecScpiCmdsProgram


def setUpModule():
    start_port = 16520
    port_cnt = 100
    PortNumberGenerator.set_port_range(start_port, start_port + port_cnt - 1)


class TestExecScpiCmds(unittest.TestCase):
    def test_program_exit_code_with_basic_commandline_arguments(self) -> None:
        # Prepare message file
        filename = MessageParser.get_filename_from_class_and_method(self, self.test_program_exit_code_with_basic_commandline_arguments) + ".txt"
        filename = os.path.join(os.path.dirname(os.path.abspath(__file__)), filename)
        lines = list()
        lines.append(" \t*IDN?| *IDN? \t ")
        lines.append("*STB?")
        lines.append("MEASURE:FFT1:UL1?")
        lines.append("TEST")
        lines.append("TEST:TEST2")
        file_writer = FileWriterHelper(filename, lines)
        file_writer.write_file()
        # Run program to send messages over TCP/IP and read reponses
        ip_address = "localhost"
        port_number = PortNumberGenerator.get_next_port_number()  # Use different port for each test to allow them being executed in parallel
        logging.debug("Starting instrument...")
        instrument = ScpiInstrumentStub(ip_address, port_number)
        instrument.run(run_in_background=True)
        try:  # Make sure to cleanup on assertion exception
            sys.argv[1:] = ["-i", ip_address, "-p", str(port_number), "-f", filename]
            logging.debug("Running program...")
            program = ExecScpiCmdsProgram()
            program.run()
        except SystemExit as e:
            self.assertEqual(e.code, 0, f"Program returned with exit code {e.code}.")
            # TODO modify program (add command line parameter to exit on errors like timeout?) to exit (with different codes) on errors
        except Exception as e:
            raise e  # Propagate exception to unittest
        finally:
            logging.debug("Cleanup...")
            instrument.quit()
            file_writer.remove_file()


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    unittest.main()
