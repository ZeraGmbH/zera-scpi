from typing import Iterable, Optional, Type
import logging
import types
import os


class FileWriterHelper:
    def __init__(self, filename: str, lines: Iterable[str], line_terminator: str = "\n") -> None:
        self._filename = filename
        self._lines = lines
        self._line_terminator = line_terminator

    def __repr__(self):
        return f"FileWriterHelper(filename=\"{self._filename}\", lines={self._lines}, line_terminator={self._line_terminator})"

    def __str__(self):
        return repr(self)

    def __enter__(self) -> None:  # There is no useful object to return
        self.remove_file()
        self.write_file()

    def __exit__(self, _exc_type: Optional[Type[BaseException]], _exc_val: Optional[BaseException], _traceback: Optional[types.TracebackType]) -> None:
        self.remove_file()

    def write_file(self) -> None:
        with open(self._filename, "a", encoding="utf-8") as file:
            for line in self._lines:
                file.write(line + self._line_terminator)

    def remove_file(self) -> None:
        if os.path.exists(self._filename):
            logging.debug("Removing file \"%s\".", self._filename)
            os.remove(self._filename)
        if os.path.exists(self._filename): # Will we ever reach this point if remove() has failed?
            logging.debug("Removal of file \"%s\" failed.", self._filename)
