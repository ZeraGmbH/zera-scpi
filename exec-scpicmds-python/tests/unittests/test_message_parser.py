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
        self.assertEqual(type(messages), list, "Read invalid number of messages.")
        self.assertEqual(len(messages), 4, "Read invalid number of messages.")
        self.assertEqual(messages[0], lines[0].strip(), f"Message #0 \"{lines[0]}\" is not parsed correctly.")
        self.assertEqual(messages[1], lines[1], f"Message #1 \"{(lines[1][:20] + '...') if len(lines[1]) < 20 else lines[1]}\" is not parsed correctly.")
        self.assertEqual(messages[2], lines[3].strip(), f"Message #3 \"{lines[3]}\" is not parsed correctly.")
        self.assertEqual(messages[3], lines[4].strip(), f"Message #4 \"{lines[4]}\" is not parsed correctly.")


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    unittest.main()
