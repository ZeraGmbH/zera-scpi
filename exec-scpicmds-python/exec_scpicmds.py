#!/usr/bin/env python

from typing import List, Optional, Tuple
import os
import sys
import time
sys.path.insert(0, '.')
import signal
import argparse
import logging
import re
import textwrap
from itertools import chain
import traceback
from importlib import import_module
from src.logging_handler import Logging, LoggingColor, LoggingStyle
from src.message_parser import MessageParser, CommandType, MessageData
from src.message_handlers import TCPHandler


class ExecScpiCmdsArgsParser:
    @staticmethod
    def _check_positive_integer(value: str, excl_zero: bool=False) -> int:
        int_value = int(value)
        if (excl_zero and int_value <= 0) or (not excl_zero and int_value < 0):
            raise argparse.ArgumentTypeError(f"{value} is not a positive int value!")
        return int_value

    @staticmethod
    def _check_port_number(value: str) -> int:
        int_value = int(value)
        if not 0 < int_value <= 65535:
            raise argparse.ArgumentTypeError(f"{value} is not a valid port number!")
        return int_value

    @staticmethod
    def _check_ip_address_or_hostname(value: str) -> str:
        ip_pattern = r"^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$"
        hostname_pattern = r"^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])$"
        if not (re.match(ip_pattern, value) or re.match(hostname_pattern, value)):
            raise argparse.ArgumentTypeError(f"{value} is not a valid ip address or hostname!")
        return value

    @staticmethod
    def parse(args: Optional[List[str]]=None) -> Optional[argparse.Namespace]:
        class _RawDescriptionHelpFormatterWithNewlines(argparse.RawDescriptionHelpFormatter):
            def _split_lines(self, text, width):
                return list(chain.from_iterable([textwrap.wrap(t, width) for t in text.splitlines()]))

        help_epilog = """\
            Plane-Input-File-Format:
              The plane input files are in text format and consist of one or more lines.
              When parsing lines, they get trimmed, which means, that leading and trailing whitespaces get removed.
              Each line is classified into following types:
              * Comment lines: Lines starting with #. These lines get ignored.
              * Empty lines: Lines containing only whitespaces. These lines get ignored.
              * SCPI message lines: All other lines. These lines get sent as SCPI messages to the instrument.

            Python-Input-File-Format:
              The python input files are python files implementing a certain interface (which is a prerequisite forbeing executed successfully):
              * Essentially it's the run() function of the ScpiScript class.
              * Basis for the interaction with the main program is the ScpiScript.send() functions and its return value. Otherwise this list will be empty.
              * This return value is a list holding the responses of all queries in the sent message, if there were such.
              * To exit the program with a certain error code, just leave ScpiScript.run() with return <ERROR_CODE>. This will then terminate the execution, clean up and exit the program with the given code. Make sure to choose values not interfering with the program's other exit values. So, it's best to use values starting at 100.
              With these files one can use functionalities provided by the ScpiScript-class, like send() (incl. response), log(), sleep(), wait_for_opc(), etc. Furthermore the whole power of python can get used, e.g. to repeat commands, make time measurements, conditional execution, etc.
              """
        parser = argparse.ArgumentParser(add_help=False, description="Send SCPI commands and receive responses.", formatter_class=_RawDescriptionHelpFormatterWithNewlines, epilog=textwrap.dedent(help_epilog))
        parser.add_argument("-h", "--help", action="help", default=argparse.SUPPRESS,
                            help="Show this help message and exit.")
        parser.add_argument("-f", "--input-file", required=True, default=argparse.SUPPRESS,
                            help="Path to file containing SCPI commands. Depending on the file extension its either interpreted as\n"
                            "* Plain SCPI command file [not *.py] (see description in section Plain-Input-File-Format below) or as\n"
                            "* Python SCPI command file [*.py] (see description in section Python-Input-File-Format below).")
        parser.add_argument("-i", "--ip-address", required=True, type=ExecScpiCmdsArgsParser._check_ip_address_or_hostname, default=argparse.SUPPRESS,
                            help="IP-address of instrument.")
        parser.add_argument("-p", "--port-number", required=True, type=ExecScpiCmdsArgsParser._check_port_number, default=argparse.SUPPRESS,
                            help="Port number of instrument.")
        parser.add_argument("-t", "--receive-timeout", type=lambda x: ExecScpiCmdsArgsParser._check_positive_integer(x, excl_zero=False), default=3000,
                            help="Receive timeout [ms] of TCP/IP-connection to instrument. If set to 0, it will wait forever (but in this case the OS timeout might trigger anyway). Default: %(default)s.")
        parser.add_argument("--enable-formatted-output", action="store_true", default=True,
                            help="Enable output formatted with colors and styles. Default: %(default)s.")
        parser.add_argument("-r", "--number-of-repetitions", type=lambda x: ExecScpiCmdsArgsParser._check_positive_integer(x, excl_zero=False), default=1,
                            help="Number of repetitions sending whole SCPI command file. Default: %(default)s.")
        parser.add_argument("-s", "--sync-cmds-with-instrument", type=int, choices=range(0, 4), default=1,
                            help="Specifies if and how the SCPI common command *OPC? is used to synchronize commands, which by their nature do not have a response. Following modes are available:\n" \
                                "0 = No synchronization used.\n" \
                                "1 [Default] = Polling using *OPC?. Split messages into its parts and handles them separately. Queries are handled normally by waiting for their corresponding response."\
                                "For each non-query an *OPC? is used to poll until the current command has finished. This mode does not depend on an activated queueing of SCPI commands on the instrument.\n" \
                                "2 = Insert a *OPC? query after each command into SCPI message. After each command in the SCPI message an |*OPC? is inserted to synchronize these commands. "\
                                "Also expects more responses, which will be hidden from the user. Only that way a timeout can get detected. "\
                                "Attention: this feature only works properly when queueing of SCPI commands on the instrument is activated.\n"\
                                "3 = Use delays for pseudo synchronization. This gives each command/message enough time to complete before the next command/message gets sent. This might help to prevent interfering with *OPC? queries for a more reliable behavior. "\
                                "See parameter --send-delays for more details on how to set these delays.")
        parser.add_argument("-d", "--send-delays", nargs=2, metavar=("COMMAND_DELAY", "MESSAGE_DELAY"), type=lambda x: ExecScpiCmdsArgsParser._check_positive_integer(x, excl_zero=False), default=[0, 0],
                            help="A delay of COMMAND_DELAY [ms] is performed after each command (i.e. not for queries) and a delay of MESSAGE_DELAY [ms] is performed after each message. Default: %(default)s.")
        try:
            if args is None:
                parser_args = parser.parse_args()
            else:
                parser_args = parser.parse_args(args)
            if parser_args.receive_timeout == 0:
                parser_args.receive_timeout = None
            return parser_args
        except SystemExit:
            return None


class ExecScpiCmdsProgram:
    @staticmethod
    def signal_handler(_sig, _frame):
        Logging.log_debug_msg("Ctrl+C pressed!", LoggingColor.RED, LoggingStyle.BOLD)
        sys.exit(0)

    @staticmethod
    def run() -> int:
        Logging.setup(log_level=logging.DEBUG)
        signal.signal(signal.SIGINT, ExecScpiCmdsProgram.signal_handler)
        args = ExecScpiCmdsArgsParser.parse()
        if args is None:
            return 1

        Logging.enable_formatted_output(args.enable_formatted_output)
        if not os.path.exists(args.input_file):
            Logging.log_debug_msg(f"File \"{args.input_file}\" does not exist!", LoggingColor.RED)
            return 2

        if (messages_and_lines := MessageParser.read_messages_with_lines_from_file(args.input_file)) is None:
            Logging.log_debug_msg(f"Reading file \"{args.input_file}\" failed!", LoggingColor.RED)
            return 3

        messages: List[MessageData] = [MessageParser.get_message_data_from_string(*message_and_line, len(messages_and_lines[0]))
                                       for message_and_line in zip(*messages_and_lines)]

        ExecScpiCmdsProgram._check_messages(messages)

        Logging.log_debug_msg(f"Connecting to {args.ip_address}:{args.port_number}...")
        tcp_handler = TCPHandler(args.ip_address, args.port_number, args.receive_timeout)
        if tcp_handler.connected:
            Logging.log_debug_msg(f"... successfully connected to {args.ip_address}:{args.port_number}.", LoggingColor.GREEN)
        else:
            Logging.log_debug_msg("... establishing connection failed!", LoggingColor.RED)
            return 4

        return_value = 0
        if not args.input_file.strip().endswith(".py"):
            ExecScpiCmdsProgram._print_and_send_messages_and_read_responses(tcp_handler, messages, args.number_of_repetitions, args.sync_cmds_with_instrument, args.receive_timeout, args.send_delays)
        else:
            return_value = ExecScpiCmdsProgram._print_and_send_messages_and_read_responses_from_python_scripting_interface(args.input_file, tcp_handler, args.number_of_repetitions, args.sync_cmds_with_instrument, args.receive_timeout, args.send_delays)

        Logging.log_debug_msg(f"Disconnecting from {args.ip_address}:{args.port_number}...")
        del tcp_handler

        return return_value

    @staticmethod
    def _check_message(message: MessageData) -> bool:
        if not message.is_valid:
            for command in message.commands:
                if command.command_type is CommandType.UNKNOWN:
                    Logging.log_debug_msg(f" > Invalid command found in line {message.file_line_number + 1}:{command.position_in_message + 1}", LoggingColor.YELLOW)
                elif command.command_type is CommandType.EMPTY:
                    Logging.log_debug_msg(f" > Empty command found in line {message.file_line_number + 1}:{command.position_in_message + 1}", LoggingColor.YELLOW)
                # TODO Check for ASCII characters and allowed symbols only: a-zA-z0-9.:,;*?-+[[:blank:]] (even more symbols?)
            return False
        return True

    @staticmethod
    def _check_messages(messages: List[MessageData]) -> None:
        Logging.log_debug_msg("Checking messages...")
        invalid_message_found = False
        for message in messages:
            if not ExecScpiCmdsProgram._check_message(message):
                invalid_message_found = True
        if not invalid_message_found:
            Logging.log_debug_msg("... all messages are valid.", LoggingColor.GREEN)
        else:
            Logging.log_debug_msg("... invalid messages(s) found!", LoggingColor.RED)

    @staticmethod
    def _send_message_and_read_responses(tcp_handler: TCPHandler, message: MessageData, indices_of_expected_responses: List[int], expected_responses_max_idx_string_width: int) -> List[str]:
        responses = []
        message_string = message.original_message
        tcp_handler.send_message(message_string + "\n")
        for resp_idx, _ in enumerate(indices_of_expected_responses):
            response = tcp_handler.receive_response()
            responses.append(response)
            if response is not None:
                Logging.log_debug_msg(f" <-[{str(indices_of_expected_responses[resp_idx] + 1).zfill(expected_responses_max_idx_string_width)}] {response}")
            else:
                Logging.log_debug_msg(f" <-[{str(indices_of_expected_responses[resp_idx] + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on receiving response.", LoggingColor.RED)
        return responses

    @staticmethod
    def wait_for_opc(tcp_handler: TCPHandler, expected_responses_max_idx_string_width: int, timeout: Optional[int], resp_idx: int) -> None:
        start = time.time()
        while True:
            tcp_handler.send_message("*OPC?\n")
            response = tcp_handler.receive_response()
            if timeout is not None:
                elapsed_time = time.time() - start
                if response is None or elapsed_time > timeout / 1000:
                    if resp_idx >= 0:
                        Logging.log_debug_msg(f" <-[{str(resp_idx + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on executing command.", LoggingColor.RED)
                    break
            if response == "+1":
                break
            time.sleep(0.01)

    @staticmethod
    def _send_split_message_and_read_responses_with_opc_polling(tcp_handler: TCPHandler, message: MessageData, expected_responses_max_idx_string_width: int, timeout: Optional[int]) -> List[str]:
        responses = []
        for resp_idx, command in enumerate(message.commands):
            tcp_handler.send_message(command.command_trimmed + "\n")
            if command.command_type is CommandType.QUERY:
                response = tcp_handler.receive_response()
                responses.append(response)
                if response is not None:
                    Logging.log_debug_msg(f" <-[{str(resp_idx + 1).zfill(expected_responses_max_idx_string_width)}] {response}")
                else:
                    Logging.log_debug_msg(f" <-[{str(resp_idx + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on receiving response.", LoggingColor.RED)
            else:
                ExecScpiCmdsProgram.wait_for_opc(tcp_handler, expected_responses_max_idx_string_width, timeout, resp_idx)
        return responses

    @staticmethod
    def _send_message_and_read_responses_with_opc(tcp_handler: TCPHandler, message: MessageData, expected_responses_max_idx_string_width: int) -> List[str]:
        responses = []
        message_string = "|".join([cmd.command if cmd.command_type is CommandType.QUERY else cmd.command + "|" + "*OPC?" for cmd in message.commands])
        tcp_handler.send_message(message_string + "\n")
        for resp_idx, _ in enumerate(message.commands):
            response = tcp_handler.receive_response()
            responses.append(response)
            if response is not None:
                if message.commands[resp_idx].command_type is CommandType.QUERY:
                    Logging.log_debug_msg(f" <-[{str(resp_idx + 1).zfill(expected_responses_max_idx_string_width)}] {response}")
            else:
                if message.commands[resp_idx].command_type is CommandType.QUERY:
                    Logging.log_debug_msg(f" <-[{str(resp_idx + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on receiving response.", LoggingColor.RED)
                else:
                    Logging.log_debug_msg(f" <-[{str(resp_idx + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on executing command.", LoggingColor.RED)
        return responses

    @staticmethod
    def _send_message_and_commands_delayed_and_read_responses(tcp_handler: TCPHandler, message: MessageData, indices_of_expected_responses: List[int], expected_responses_max_idx_string_width: int, send_delays: Tuple[int, int]) -> List[str]:
        responses = []
        if send_delays[0] <= 0:  # No delay, so send normally
            message_string = message.original_message
            tcp_handler.send_message(message_string + "\n")
            for resp_idx, _ in enumerate(indices_of_expected_responses):
                response = tcp_handler.receive_response()
                responses.append(response)
                if response is not None:
                    Logging.log_debug_msg(f" <-[{str(indices_of_expected_responses[resp_idx] + 1).zfill(expected_responses_max_idx_string_width)}] {response}")
                else:
                    Logging.log_debug_msg(f" <-[{str(indices_of_expected_responses[resp_idx] + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on receiving response.", LoggingColor.RED)
        else:  # Delay, so splitting message is necessary
            for resp_idx, command in enumerate(message.commands):
                tcp_handler.send_message(command.command_trimmed + "\n")
                if command.command_type is CommandType.QUERY:
                    response = tcp_handler.receive_response()
                    responses.append(response)
                    if response is not None:
                        Logging.log_debug_msg(f" <-[{str(resp_idx + 1).zfill(expected_responses_max_idx_string_width)}] {response}")
                    else:
                        Logging.log_debug_msg(f" <-[{str(resp_idx + 1).zfill(expected_responses_max_idx_string_width)}] Timeout on receiving response.", LoggingColor.RED)
                else:
                    time.sleep(send_delays[0] / 1000)
        if send_delays[1] > 0:
            time.sleep(send_delays[1] / 1000)
        return responses

    @staticmethod
    def _send_message_and_read_responses_variants(tcp_handler: TCPHandler, message: str, sync_cmds_with_instrument: int, timeout: Optional[int], send_delays : Tuple[int, int]) -> List[str]:
        message_part_indices_string_width = len(str(len(message.commands)))
        indices_of_expected_responses = [idx for idx, command in enumerate(message.commands) if command.command_type is CommandType.QUERY]
        expected_responses_max_idx_string_width = len(str(max(indices_of_expected_responses) + 1)) if len(indices_of_expected_responses) > 0 else 0
        ExecScpiCmdsProgram._print_message_with_part_indices(message, message_part_indices_string_width)
        if sync_cmds_with_instrument == 0:
            return ExecScpiCmdsProgram._send_message_and_read_responses(tcp_handler, message, indices_of_expected_responses, expected_responses_max_idx_string_width)
        if sync_cmds_with_instrument == 1:
            return ExecScpiCmdsProgram._send_split_message_and_read_responses_with_opc_polling(tcp_handler, message, expected_responses_max_idx_string_width, timeout)
        if sync_cmds_with_instrument == 2:
            return ExecScpiCmdsProgram._send_message_and_read_responses_with_opc(tcp_handler, message, expected_responses_max_idx_string_width)
        if sync_cmds_with_instrument == 3:
            return ExecScpiCmdsProgram._send_message_and_commands_delayed_and_read_responses(tcp_handler, message, indices_of_expected_responses, expected_responses_max_idx_string_width, send_delays)
        return []

    @staticmethod
    def _print_and_send_messages_and_read_responses(tcp_handler: TCPHandler, messages: List[str], number_of_repetitions: int, sync_cmds_with_instrument: int, timeout: Optional[int], send_delays : Tuple[int, int]) -> None:
        if len(messages) > 0:
            for current_repetition in range(number_of_repetitions):
                ExecScpiCmdsProgram._print_sending_messages(current_repetition, number_of_repetitions)
                for message in messages:
                    ExecScpiCmdsProgram._send_message_and_read_responses_variants(tcp_handler, message, sync_cmds_with_instrument, timeout, send_delays)
        else:
            Logging.log_debug_msg("No messages to send.", LoggingColor.BLUE)

    @staticmethod
    def load_scpi_script(input_file: str) -> Optional[object]:
        # pylint: disable=too-many-return-statements
        scpi_script_name = "src/scpi_script.py"  # The internally used module filename
        run_function_name = "run"
        exec_script_path = os.path.dirname(os.path.abspath(__file__))
        scpi_script_path = os.path.join(exec_script_path, scpi_script_name)
        if os.path.islink(scpi_script_path):
            os.remove(scpi_script_path)
            if os.path.exists(scpi_script_path):
                Logging.log_debug_msg(f"Removing internal placeholder link \"{scpi_script_path}\" for SCPI scripting file was not successful.", LoggingColor.RED)
                return None
        else:
            if os.path.exists(scpi_script_path):
                Logging.log_debug_msg(f"Internal placeholder \"{scpi_script_path}\" for SCPI scripting file is not a link and therefore will not get deleted.", LoggingColor.RED)
                return None
        os.symlink(os.path.abspath(input_file), scpi_script_path)
        if not os.path.islink(scpi_script_path):
            Logging.log_debug_msg(f"Creating internal placeholder link \"{scpi_script_path}\" for SCPI scripting file was not successful.", LoggingColor.RED)
            return None
        try:
            scpi_script_module = import_module(os.path.splitext(scpi_script_name)[0].replace("/", "."))
            scpi_script_object_name = scpi_script_module.ScpiScript.__name__
        except (ModuleNotFoundError, ImportError):
            Logging.log_debug_msg(f"Importing SCPI scripting module from internal placeholder link \"{scpi_script_path}\" was not successful.", LoggingColor.RED)
            return None
        try:
            ScpiScript = getattr(scpi_script_module, scpi_script_object_name)  # pylint: disable=invalid-name
        except AttributeError:
            Logging.log_debug_msg(f"Loading class \"{scpi_script_object_name}\" from SCPI scripting module from internal placeholder link \"{scpi_script_path}\" was not successful.", LoggingColor.RED)
            return None
        if not hasattr(ScpiScript, run_function_name) or run_function_name in ScpiScript.__abstractmethods__ or not callable(getattr(ScpiScript, run_function_name)):
            Logging.log_debug_msg(f"The class \"{scpi_script_object_name}\" from SCPI scripting module from internal placeholder link \"{scpi_script_path}\" has no function named \"{run_function_name}\".", LoggingColor.RED)
            return None
        return ScpiScript

    @staticmethod
    def _print_and_send_messages_and_read_responses_from_python_scripting_interface(input_file: str, tcp_handler: TCPHandler, number_of_repetitions: int, sync_cmds_with_instrument: int, timeout: Optional[int], send_delays : Tuple[int, int]) -> int:
        def send_receive_message(message_string: str, file_line_number: int) -> List[str]:
            message = MessageParser.get_message_data_from_string(message_string, file_line_number, 1)
            ExecScpiCmdsProgram._check_message(message)
            return ExecScpiCmdsProgram._send_message_and_read_responses_variants(tcp_handler, message, sync_cmds_with_instrument, timeout, send_delays)
        def wait_for_opc() -> None:
            ExecScpiCmdsProgram.wait_for_opc(tcp_handler, 1, timeout, -1)
        def log(message: str, color: LoggingColor=LoggingColor.NONE, style: LoggingStyle=LoggingStyle.NONE) -> None:
            Logging.log_debug_msg(message, color, style)
        if (ScpiScript := ExecScpiCmdsProgram.load_scpi_script(input_file)) is not None:  # pylint: disable=invalid-name
            try:
                for current_repetition in range(number_of_repetitions):
                    ExecScpiCmdsProgram._print_sending_messages(current_repetition, number_of_repetitions)
                    return_value = ScpiScript(send_callback=send_receive_message, wait_for_opc_callback=wait_for_opc, log_callback=log).run()
                    if return_value is not None and return_value > 0:
                        return return_value
            except Exception as exception:  # pylint: disable=broad-exception-caught
                Logging.log_debug_msg(f"While executing SCPI scripting, an error occurred: {exception}.", LoggingColor.RED)
                trace_back = traceback.format_exc()
                for line in trace_back.split("\n"):
                    Logging.log_debug_msg(f"{line}", LoggingColor.YELLOW)
        else:
            Logging.log_debug_msg("Stopping execution.", LoggingColor.RED)
            return 5
        return 0

    @staticmethod
    def _print_sending_messages(current_repetition: int, number_of_repetitions: int) -> None:
        if number_of_repetitions == 1:
            Logging.log_debug_msg("Sending messages...", LoggingColor.BLUE)
        else:
            Logging.log_debug_msg(f"Sending messages (#{current_repetition + 1}/{number_of_repetitions})...", LoggingColor.BLUE)

    @staticmethod
    def _print_message_with_part_indices(message: MessageData, expected_responses_count_string_width: int) -> None:
        line_number_string = str(message.file_line_number + 1).zfill(message.command_count_string_width)
        Logging.log_debug_msg(f"==> [L{line_number_string}] {message.original_message}", style=LoggingStyle.BOLD)
        message_part_position = ""
        for cmd_idx, command in enumerate(message.commands):
            message_part_position = message_part_position.ljust(command.position_in_message, " ")
            message_part_position += f"[{str(cmd_idx + 1).zfill(expected_responses_count_string_width)}]"
        Logging.log_debug_msg(f"{''.ljust(len(line_number_string) + 8)}{message_part_position}", LoggingColor.GREEN)


def main() -> None:
    program = ExecScpiCmdsProgram
    result = program.run()
    sys.exit(result)


if __name__ == "__main__":
    main()
