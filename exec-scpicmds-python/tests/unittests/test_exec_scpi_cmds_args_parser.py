import unittest
import logging
import sys
sys.path.insert(0, '.')
import shlex
from exec_scpicmds import ExecScpiCmdsArgsParser


def setUpModule():  # pylint: disable=invalid-name
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")


class TestExecScpiCmdsArgsParser(unittest.TestCase):
    def test_parse_command_line_arguments(self) -> None:
        i = "localhost"      # ip_address  # pylint: disable=invalid-name
        p = 6320             # port_number # pylint: disable=invalid-name
        f = "scpi_cmds.txt"  # filename    # pylint: disable=invalid-name
        # Note: shlex.split() cannot properly split short options with values that are not separated by space(s), e.g. -ilocalhost
        argv = shlex.split(f"-i {i} -p {p} -f 'space test {f}'")
        sys.argv[1:] = argv
        self.assertEqual(ExecScpiCmdsArgsParser.parse(), ExecScpiCmdsArgsParser.parse(argv), "Parsing args with sys.argv and as function argument are not equal.")
        self.assertNotEqual(ExecScpiCmdsArgsParser.parse(), None, f"Parsing args with sys.argv {sys.argv[1:]} failed.")
        valid_cmd_lines = []
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}'")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' -t 0")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' -t 50000")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' -r 0")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' -r 1")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' -r 1000")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --number-of-repetitions 1000")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --sync-cmds-with-instrument 0")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --sync-cmds-with-instrument 1")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' -s 2")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' -s 5")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --send-delays 0 0")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' -d 5000 0")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' -d 0 1000")
        valid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --enable-formatted-output")

        for valid_cmd_line in valid_cmd_lines:
            self.assertNotEqual(ExecScpiCmdsArgsParser.parse(shlex.split(valid_cmd_line)), None, f"Parsing args \"{valid_cmd_line}\" failed.")

        invalid_cmd_lines = []
        invalid_cmd_lines.append(f"-i {i} -p {0}")
        invalid_cmd_lines.append(f"-i {i} -f '{f}'")
        invalid_cmd_lines.append(f"-p {0} -f '{f}'")
        invalid_cmd_lines.append(f"-i {i} -p {0} -f '{f}'")
        invalid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' -t a")
        invalid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' -r a")
        invalid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --sync-cmds-with-instrument")
        invalid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --sync-cmds-with-instrument 6")
        invalid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --send-delays")
        invalid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --send-delays 0")
        invalid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --send-delays 0 0 0")
        invalid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --send-delays a")
        invalid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --enable-formatted-output 0")
        invalid_cmd_lines.append(f"-i {i} -p {p} -f '{f}' --enable-formatted-output 1")
        for invalid_cmd_line in invalid_cmd_lines:
            self.assertEqual(ExecScpiCmdsArgsParser.parse(shlex.split(invalid_cmd_line)), None, f"Parsing args \"{invalid_cmd_line}\" unexpectedly succeeded.")
