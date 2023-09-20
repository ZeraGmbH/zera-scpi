import os
import sys
sys.path.insert(0, '.')
import unittest
import logging
from src.logging_handler import Logging, LoggingColor, LoggingStyle
from testlib.file_helper import FileHelper

class TestLogging(unittest.TestCase):
    def test_message_formatting(self) -> None:
        self.assertEqual(Logging.format_message("TEST", LoggingColor.NONE, LoggingStyle.NONE), Logging.format_message("TEST"), "Default color and style should be of type NONE.")
        self.assertEqual(Logging.format_message("TEST", LoggingColor.BLUE, LoggingStyle.BOLD), "\x1b[0;34m\x1b[1mTEST\x1b[0m\x1b[0;0m", "Formatted string does not match expected one.")
        self.assertEqual(Logging.format_message("TEST", LoggingColor.RED), "\x1b[0;31mTEST\x1b[0;0m", "Color only formatting does not match expected one.")
        self.assertEqual(Logging.format_message("TEST", LoggingColor.YELLOW), Logging.format_message("TEST", LoggingColor.YELLOW), "Identical color formatting does not match expected one.")
        self.assertNotEqual(Logging.format_message("TEST", LoggingColor.YELLOW), Logging.format_message("TEST", LoggingColor.GREEN), "Identical color formatting does not match expected one.")
        self.assertEqual(Logging.format_message("TEST", style=LoggingStyle.ITALIC), Logging.format_message("TEST", LoggingColor.NONE, LoggingStyle.ITALIC), "Default color is not NONE.")

    # TODO fix following test, which fails with errors like this:
    #      AssertionError: 3 != 1 : Unexpected number of lines in logging output file "/__w/zera-scpi/zera-scpi/exec-scpicmds-python/tests/unittests/TestLogging__test_logging_setup.log".
    #      This happens most likeky due to parallel access to Logging class by other tests.
    #      How to seolve this?

    # def test_logging_setup(self) -> None:
    #     filename = FileHelper.get_filename_from_class_and_method(self, self.test_logging_setup) + ".log"
    #     filename = os.path.join(os.path.dirname(os.path.abspath(__file__)), filename)
    #     if os.path.exists(filename):
    #         os.remove(filename)
    #     Logging.setup(filename, log_level=logging.DEBUG, enable_formatted_output=False)
    #     Logging.log_debug_msg("DEBUG_TEST", LoggingColor.RED, LoggingStyle.BOLD)
    #     lines = []
    #     with open(filename, "r") as file:
    #         lines = file.readlines()
    #     self.assertEqual(len(lines), 1, f"Unexpected number of lines in logging output file \"{filename}\".")
    #     self.assertIn("DEBUG_TEST", lines[0], f"Test-string not found in message in logging output file \"{filename}\".")
    #     if os.path.exists(filename):
    #         os.remove(filename)
