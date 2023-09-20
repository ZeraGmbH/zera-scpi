import unittest
import logging
import sys
sys.path.insert(0, '.')
import shlex
from src.exec_scpicmds import ExecScpiCmdsArgsParser


class TestExecScpiCmdsArgsParser(unittest.TestCase):
    def test_parse_command_line_arguments(self) -> None:
        i = "localhost"      # ip_address
        p = 6320             # port_number
        f = "scpi_cmds.txt"  # filename
        argv = shlex.split(f"-i {i} -p {p} -f 'space test {f}'")
        sys.argv[1:] = argv
        self.assertEqual(ExecScpiCmdsArgsParser.parse(), ExecScpiCmdsArgsParser.parse(argv), f"Parsing args with sys.argv and as function argument are not equal.")
        self.assertNotEqual(ExecScpiCmdsArgsParser.parse(), None, f"Parsing args with sys.argv {sys.argv[1:]} failed.")
        valid_cmd_lines = list()
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}'")
        for valid_cmd_line in valid_cmd_lines:
            self.assertNotEqual(ExecScpiCmdsArgsParser.parse(shlex.split(valid_cmd_line)), None, f"Parsing args \"{valid_cmd_line}\" failed.")
        
        invalid_cmd_lines = list()
        invalid_cmd_lines.append(f"-i {i} -p {0} -f '{f}'")
        invalid_cmd_lines.append(f"-i {i} -p {1} -f '{f}'")  # TODO fix temp. CI fail test
        for invalid_cmd_line in invalid_cmd_lines:
            self.assertEqual(ExecScpiCmdsArgsParser.parse(shlex.split(invalid_cmd_line)), None, f"Parsing args \"{invalid_cmd_line}\" unexpectedly succeeded.")


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    unittest.main()
