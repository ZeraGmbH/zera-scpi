import unittest
import logging
import os
from src.message_parser import MessageParser


class TestMessageParser(unittest.TestCase):
    def test_read_messages_from_file(self):
        filename = "example_file_for_test_read_messages_from_file.txt"
        if os.path.exists(filename):
            logging.debug(f"Removing file \"{filename}\".")
            os.remove(filename)
        if os.path.exists(filename):
            logging.debug(f"Removal of file \"{filename}\" failed.")
            self.assertFalse(True, f"File {filename} already exists and couldn't get removed. Therefore no tests can get executed.")
        else:
            lines = list()
            lines.append("\t*IDN? |IDN?  ")
            lines.append("|".join(["*IDN" for x in range(1000)]))
            lines.append("\t")
            lines.append("*STB?")
            lines.append("TEST")
            lines.append("# My comment")

            file = open(filename, "a")
            for line in lines:
                file.write(line + "\n")
            file.close()

            messages = MessageParser.read_messages_from_file(filename)

            self.assertEqual(type(messages), list, "Read invalid number of messages.")
            self.assertEqual(len(messages), 4, "Read invalid number of messages.")
            self.assertEqual(messages[0], lines[0].strip(), f"Message #0 \"{lines[0]}\" is not parsed correctly.")
            self.assertEqual(messages[1], lines[1], f"Message #1 \"{(lines[1][:20] + '...') if len(line[1]) < 20 else lines[1]}\" is not parsed correctly.")
            self.assertEqual(messages[2], lines[3].strip(), "Message #3 \"{lines[3]}\" is not parsed correctly.")
            self.assertEqual(messages[3], lines[4].strip(), "Message #4 \"{lines[4]}\" is not parsed correctly.")

            if os.path.exists(filename):
                os.remove(filename)


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    unittest.main()
