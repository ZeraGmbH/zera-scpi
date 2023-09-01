from typing import Optional


class MessageParser:
    @staticmethod
    def read_messages_from_file(file_path: str) -> Optional[list[str]]:
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
