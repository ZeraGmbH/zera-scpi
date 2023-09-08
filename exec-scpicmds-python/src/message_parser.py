from typing import Optional, List, Tuple


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
