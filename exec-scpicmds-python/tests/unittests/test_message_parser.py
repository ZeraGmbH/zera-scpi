import unittest
import logging
import os
import sys
sys.path.insert(0, '.')
from src.message_parser import MessageParser
from testlib.message_parser_helper import FileWriterHelper


class TestMessageParser(unittest.TestCase):
    def test_read_messages_from_file(self) -> None:
        filename = MessageParser.get_filename_from_class_and_method(self, self.test_read_messages_from_file) + ".txt"
        filename = os.path.join(os.path.dirname(os.path.abspath(__file__)), filename)
        lines = list()
        lines.append("\t*IDN? |IDN?  ")
        lines.append("|".join(["*IDN" for x in range(1000)]))
        lines.append("\t")
        lines.append("*STB?")
        lines.append("TEST")
        lines.append("# My comment")
        messages = None
        try:
            with FileWriterHelper(filename, lines):
                try:
                    messages = MessageParser.read_messages_from_file(filename)
                except Exception as e:
                    self.assertTrue(False, f"Reading temporary test file \"{filename}\" failed with exception \"{e}\".")
        except Exception as e:
            self.assertTrue(False, f"Writing temporary test file \"{filename}\" failed with exception \"{e}\".")
        self.assertEqual(type(messages), list, "Return value is not a list.")
        self.assertEqual(len(messages), 4, "Read invalid number of messages.")
        self.assertEqual(messages[0], lines[0].strip(), f"Message #0 \"{lines[0]}\" is not parsed correctly.")
        self.assertEqual(messages[1], lines[1], f"Message #1 \"{(lines[1][:20] + '...') if len(lines[1]) < 20 else lines[1]}\" is not parsed correctly.")
        self.assertEqual(messages[2], lines[3].strip(), f"Message #3 \"{lines[3]}\" is not parsed correctly.")
        self.assertEqual(messages[3], lines[4].strip(), f"Message #4 \"{lines[4]}\" is not parsed correctly.")

    def test_read_messages_with_lines_from_file(self) -> None:
        filename = MessageParser.get_filename_from_class_and_method(self, self.test_read_messages_with_lines_from_file) + ".txt"
        filename = os.path.join(os.path.dirname(os.path.abspath(__file__)), filename)
        lines = list()
        lines.append("# My comment")
        lines.append("\t*IDN? |IDN?  ")
        lines.append("\t")
        lines.append("*STB?")
        messages_and_lines = None
        try:
            with FileWriterHelper(filename, lines):
                try:
                    messages_and_lines = MessageParser.read_messages_with_lines_from_file(filename)
                except Exception as e:
                    self.assertTrue(False, f"Reading temporary test file \"{filename}\" failed with exception \"{e}\".")
        except Exception as e:
            self.assertTrue(False, f"Writing temporary test file \"{filename}\" failed with exception \"{e}\".")
        self.assertNotEqual(messages_and_lines, None, "The function returned None (due to an exception).")
        messages, line_numbers = messages_and_lines
        self.assertEqual(len(messages), 2, "Read invalid number of messages/lines.")
        lines_with_real_messages = [1, 3]
        for idx_read, idx_ori in enumerate(lines_with_real_messages):
            self.assertEqual((messages[idx_read], line_numbers[idx_read]), (lines[idx_ori].strip(), idx_ori), f"Message \"{lines[idx_ori].strip()}\" and its line number #{idx_ori} is not parsed correctly.")



if __name__ == "__main__":
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    unittest.main()
