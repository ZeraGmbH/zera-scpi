from typing import Optional, List


class MessageParser:
    @staticmethod
    def read_messages_from_file(file_path: str) -> Optional[List[str]]:
        try:
            with open(file_path, "r") as file:
                lines = list()
                while line := file.readline():
                    line_stipped = line.strip()
                    if len(line_stipped) > 0 and line_stipped[0] != '#':
                        lines.append(line.replace("\r", "").replace("\n", "").strip())
                return lines
        except OSError:
            return None

    @staticmethod
    def get_filename_from_class_and_method(c: type, m: callable):
        return c.__class__.__name__ + "__" + m.__name__
