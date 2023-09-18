from typing import Optional
import logging
from enum import Enum


class LoggingColor(Enum):
    NONE = 1,
    RED = 2,
    GREEN = 3,
    BLUE = 4,
    YELLOW = 5,


class LoggingStyle(Enum):
    NONE = 1,
    BOLD = 2,
    ITALIC = 3,
    UNDERLINE = 4,
    STRIKETHROUGH = 5,


class Logging():
    _logger = None
    _enable_formatted_output = False

    @classmethod
    def setup(cls, log_filename: Optional[str]=None, log_level: int=logging.INFO, enable_formatted_output: bool=False) -> None:
        cls._logger = logging.getLogger()
        cls._logger.setLevel(log_level)
        cls._enable_formatted_output = enable_formatted_output
        format = "[%(asctime)s.%(msecs)03d]: %(message)s"
        date_format = "%H:%M:%S"

        if log_filename is not None:
            file_handler = logging.FileHandler(log_filename)
            file_handler.setFormatter(logging.Formatter(fmt=format, datefmt=date_format))
            cls._logger.addHandler(file_handler)

        console_handler = logging.StreamHandler()
        console_handler.setFormatter(logging.Formatter(fmt=format, datefmt=date_format))
        cls._logger.addHandler(console_handler)

    @classmethod
    def enable_formatted_output(cls, enable: bool) -> None:
        cls._enable_formatted_output = enable

    @staticmethod
    def format_message(message: str, color: LoggingColor=LoggingColor.NONE, style: LoggingStyle=LoggingStyle.NONE) -> str:
        color_pre = ""
        color_post = ""
        style_pre = ""
        style_post = ""

        if color is LoggingColor.NONE:
            pass
        elif color is LoggingColor.RED:
            color_pre = "\033[0;31m"
            color_post = "\033[0;0m"
        elif color is LoggingColor.GREEN:
            color_pre = "\033[0;32m"
            color_post = "\033[0;0m"
        elif color is LoggingColor.BLUE:
            color_pre = "\033[0;34m"
            color_post = "\033[0;0m"
        elif color is LoggingColor.YELLOW:
            color_pre = "\033[0;33m"
            color_post = "\033[0;0m"
        else:
            pass

        if style is LoggingStyle.NONE:
            pass
        elif style is LoggingStyle.BOLD:
            style_pre = "\033[1m"
            style_post = "\033[0m"
        elif style is LoggingStyle.ITALIC:
            style_pre = "\033[3m"
            style_post = "\033[0m"
        elif style is LoggingStyle.UNDERLINE:
            style_pre = "\033[4m"
            style_post = "\033[0m"
        elif style is LoggingStyle.STRIKETHROUGH:
            style_pre = "\033[9m"
            style_post = "\033[0m"
        else:
            pass

        return f"{color_pre}{style_pre}{message}{style_post}{color_post}"

    @classmethod
    def log_msg(cls, message: str, level: int, color: LoggingColor=LoggingColor.NONE, style: LoggingStyle=LoggingStyle.NONE):
        if cls._logger is not None:
            if cls._enable_formatted_output:
                message = cls.format_message(message, color=color, style=style)
            cls._logger.log(level, message)

    @classmethod
    def log_debug_msg(cls, message: str, color: LoggingColor=LoggingColor.NONE, style: LoggingStyle=LoggingStyle.NONE):
        cls.log_msg(message, logging.DEBUG, color, style)
