from typing import Optional, List, Tuple
from enum import Enum
from dataclasses import dataclass, field


class CommandType(Enum):
    UNKNOWN = 1,
    COMMAND = 2,
    QUERY = 3,
    EMPTY = 4,


@dataclass
class CommandData:
    command: str = ""
    command_trimmed: str = ""
    command_type: CommandType = CommandType.UNKNOWN
    position_in_message: int = 1
    response: Optional[str] = None  # Only for queries


@dataclass
class MessageData:
    original_message: str = ""
    file_line_number: int = -1
    is_valid: bool = True
    commands: List[CommandData] = field(default_factory=list)
    command_count_string_width: int = -1


class MessageParser:
    @staticmethod
    def read_messages_from_file(file_path: str) -> Optional[List[str]]:
        result = MessageParser.read_messages_with_lines_from_file(file_path)
        if result is not None:
            lines, _ = result
            return lines
        else:
            return None

    @staticmethod
    def read_messages_with_lines_from_file(file_path: str) -> Optional[Tuple[List[str], List[int]]]:
        try:
            with open(file_path, "r") as file:
                lines = list()
                line_numbers = list()
                current_line_number = 0
                while line := file.readline():
                    line_stipped = line.strip()
                    if len(line_stipped) > 0 and line_stipped[0] != '#':
                        lines.append(line.replace("\r", "").replace("\n", "").strip())
                        line_numbers.append(current_line_number)
                    current_line_number += 1
                return lines, line_numbers
        except OSError:
            return None

    @staticmethod
    def get_filename_from_class_and_method(c: type, m: callable):
        return c.__class__.__name__ + "__" + m.__name__

    @staticmethod
    def get_command_type_from_string(command: str) -> CommandType:
        if "?" in command:
            return CommandType.QUERY
        elif len(command.strip()) == 0:
            return CommandType.EMPTY
        else:
            return CommandType.COMMAND

    @staticmethod
    def check_message_is_valid(message_data: MessageData) -> bool:
        if len(message_data.commands) == 0:
            return False
        for command in message_data.commands:
            if command.command_type in [CommandType.EMPTY, CommandType.UNKNOWN]:
                return False
        return True

    @staticmethod
    def get_message_data_from_string(message: str, file_line_number: int) -> MessageData:
        message_data = MessageData()
        commands = message.split("|")
        last_command_position = 0
        for command in commands:
            command_data = CommandData()
            message_data.commands.append(command_data)
            command_data.command = command
            command_data.command_trimmed = command.strip()
            command_data.command_type = MessageParser.get_command_type_from_string(command)
            command_data.position_in_message = last_command_position
            last_command_position = message.find("|", last_command_position) + 1
        message_data.original_message = message
        message_data.file_line_number = file_line_number
        message_data.command_count_string_width = len(str(len(commands)))
        message_data.is_valid = MessageParser.check_message_is_valid(message_data)
        return message_data
