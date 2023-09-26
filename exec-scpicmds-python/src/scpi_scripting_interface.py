from typing import Optional, List, Callable
from abc import ABC, abstractmethod
import time
from inspect import currentframe, getframeinfo
from src.logging_handler import LoggingColor, LoggingStyle


class IScpiScripting(ABC):
    def __init__(self, send_callback: Callable[[str, int], List[str]], wait_for_opc_callback: Callable[[bool], None], log_callback: Callable[[str, LoggingColor, LoggingStyle], None]) -> None:
        self._send_callback = send_callback
        self._wait_for_opc_callback = wait_for_opc_callback
        self._log_callback = log_callback

    def send(self, message: str) -> Optional[List[str]]:
        if len(message.strip()) > 0:
            return self._send_callback(message, getframeinfo(currentframe().f_back).lineno - 1)
        return None

    def log(self, message: str, color: LoggingColor=LoggingColor.NONE, style: LoggingStyle=LoggingStyle.NONE) -> None:
        self._log_callback(message, color, style)

    def sleep(self, seconds: float) -> None:
        time.sleep(seconds)

    def wait_for_opc(self, log_polling: bool=False) -> None:
        self._wait_for_opc_callback(log_polling)

    @abstractmethod
    def run(self) -> int:
        raise NotImplementedError
