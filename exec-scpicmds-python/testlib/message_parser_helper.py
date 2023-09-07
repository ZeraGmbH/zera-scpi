from typing import Iterable, Optional, Type
import logging
import types
import os


class FileWriterHelper:
    def __init__(self, filename: str, lines: Iterable[str], line_terminator: str = "\n") -> None:
        self._filename = filename
        self._lines = lines
        self._line_terminator = line_terminator

    def __enter__(self) -> None:
        self.remove_file()
        self.write_file()
        return None  # There is no useful object to return

    def __exit__(self, _exc_type: Optional[Type[BaseException]], _exc_val: Optional[BaseException], _traceback: Optional[types.TracebackType]) -> None:
        self.remove_file()
    
    def write_file(self) -> None:
        file = open(self._filename, "a")
        for line in self._lines:
            file.write(line + self._line_terminator)
        file.close()

    def remove_file(self) -> None:
        if os.path.exists(self._filename):
            logging.debug(f"Removing file \"{self._filename}\".")
            os.remove(self._filename)
        if os.path.exists(self._filename): # Will we ever reach this point if remove() has failed?
            logging.debug(f"Removal of file \"{self._filename}\" failed.")
