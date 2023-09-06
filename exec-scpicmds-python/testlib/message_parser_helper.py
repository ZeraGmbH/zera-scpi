from typing import Iterable
import logging
import os


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
