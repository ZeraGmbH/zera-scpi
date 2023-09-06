from typing import Iterable
import unittest
import logging
import os
from src.message_parser import MessageParser


class FileWriterHelper:
    def __init__(self, filename: str, lines: Iterable[str], line_terminator: str = "\n"):
        self._filename = filename
        self._lines = lines
        self._line_terminator = line_terminator

    def __enter__(self) -> None:
        self._remove_file()
        file = open(self._filename, "a")
        for line in self._lines:
            file.write(line + self._line_terminator)
        file.close()
        return None  # There is no useful object to return
    
    def __exit__(self, _exc_type, _exc_val, _traceback) -> None:
        self._remove_file()
    
    def _remove_file(self):
        if os.path.exists(self._filename):
            logging.debug(f"Removing file \"{self._filename}\".")
            os.remove(self._filename)
        if os.path.exists(self._filename): # Will we ever reach this point if remove() has failed?
            logging.debug(f"Removal of file \"{self._filename}\" failed.")


class TestMessageParser(unittest.TestCase):
    def test_read_messages_from_file(self):
        filename = "example_file_for_test_read_messages_from_file.txt"
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
        self.assertEqual(messages[2], lines[3].strip(), "Message #3 \"{lines[3]}\" is not parsed correctly.")
        self.assertEqual(messages[3], lines[4].strip(), "Message #4 \"{lines[4]}\" is not parsed correctly.")


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    unittest.main()
