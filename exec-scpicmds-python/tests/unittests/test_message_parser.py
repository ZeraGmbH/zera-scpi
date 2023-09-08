import unittest
import logging
import os
import sys
sys.path.insert(0, '.')
from src.message_parser import MessageParser, CommandType, CommandData, MessageData
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

    def test_get_command_type_from_string(self) -> None:
        self.assertEqual(MessageParser.get_command_type_from_string("TEST?"), CommandType.QUERY, f"Wrong command type returned.")
        self.assertEqual(MessageParser.get_command_type_from_string("TEST"), CommandType.COMMAND, f"Wrong command type returned.")
        self.assertEqual(MessageParser.get_command_type_from_string("TEST myparam;"), CommandType.COMMAND, f"Wrong command type returned.")
        self.assertEqual(MessageParser.get_command_type_from_string("  "), CommandType.EMPTY, f"Wrong command type returned.")

    def test_check_message_is_valid(self) -> None:
        self.assertCountEqual(CommandType, [CommandType.UNKNOWN, CommandType.COMMAND, CommandType.QUERY, CommandType.EMPTY], f"Not all enum values are tested.")
        for command_types in [[CommandType.COMMAND], [CommandType.QUERY], [CommandType.COMMAND, CommandType.QUERY]]:
            message_data = MessageData()
            message_data.commands=[CommandData("", "", command_type) for command_type in command_types]
            self.assertTrue(MessageParser.check_message_is_valid(message_data), f"Validity-check of message data incorrectly returned a negative result.")
        for command_types in [[], [CommandType.EMPTY], [CommandType.UNKNOWN], [CommandType.COMMAND, CommandType.EMPTY], [CommandType.QUERY, CommandType.UNKNOWN]]:
            message_data = MessageData()
            message_data.commands=[CommandData("", "", command_type) for command_type in command_types]
            self.assertFalse(MessageParser.check_message_is_valid(message_data), f"Validity-check of message data incorrectly returned a negative result.")

    def test_get_message_data_from_string(self) -> None:
        message = " \t *IDN? | TEST || |  TEST \tmyparam;| "
        line_number = 42
        message_data = MessageParser.get_message_data_from_string(message, line_number, 1)
        self.assertFalse(message_data.is_valid, f"Message data: is_valid-property parsed incorrectly.")
        self.assertEqual(message_data.file_line_number, line_number, f"Message data: file_line_number-property parsed incorrectly.")
        self.assertEqual(message_data.original_message, message, f"Message data: original_message-property parsed incorrectly.")
        self.assertEqual(message_data.command_count_string_width, len(str(len(message_data.commands))), f"Message data: command_count_string_width-property parsed incorrectly.")
        self.assertEqual(len(message_data.commands), 6, "Invalid number of commands in message.")
        expected_results = [[" \t *IDN? ", "*IDN?", CommandType.QUERY, 0],
                            [" TEST ", "TEST", CommandType.COMMAND, 10],
                            ["", "", CommandType.EMPTY, 17],
                            [" ", "", CommandType.EMPTY, 18],
                            ["  TEST \tmyparam;", "TEST \tmyparam;", CommandType.COMMAND, 20],
                            [" ", "", CommandType.EMPTY, 37]]
        for r, result in enumerate(expected_results):
            self.assertEqual(message_data.commands[r].command, result[0], f"Command data {r}: command-property parsed incorrectly.")
            self.assertEqual(message_data.commands[r].command_trimmed, result[1], f"Command data {r}: command_trimmed-property parsed incorrectly.")
            self.assertEqual(message_data.commands[r].command_type, result[2], f"Command data {r}: command_type-property parsed incorrectly.")
            self.assertEqual(message_data.commands[r].position_in_message, result[3], f"Command data {r}: position_in_message-property parsed incorrectly.")


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.DEBUG)
    logging.basicConfig(format="%(message)s")
    unittest.main()
