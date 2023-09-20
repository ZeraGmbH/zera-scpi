import sys
sys.path.insert(0, '.')
import socket
import random
import itertools
from testlib.tcp_server_helper import VerboseTcpServer



# This class only handles a subset of the commands and of the syntax (e.g. queries without parameters) and returns pseudo responses
class ScpiInstrumentStub(VerboseTcpServer):
    def __init__(self, ip_address: str, port_number: int) -> None:
        VerboseTcpServer.__init__(self, ip_address, port_number)

    def __repr__(self):
        return f"ScpiInstrumentStub(ip_address=\"{self._ip_address}\", port_number={self._port_number})"

    def __str__(self):
        return repr(self)

    @staticmethod
    def _send(client: socket.socket, response: str) -> None:
        client.send(response.encode())

    def on_client_receive_message(self, client: socket.socket, message: str) -> None:
        super().on_client_receive_message(client, message)
        def send(response: str) -> None:
            client.send(response.encode())
        commands = [command.strip().upper() for command in message.split("|")]
        for command in commands:
            if command.endswith("?"):
                if command == "*IDN?":
                    send("ZERA SCPI Instrument Stub version 0.0.1\n")
                elif command == "*STB?":
                    send("+0\n")
                elif command in [":".join(x) + "?" for x in itertools.product(("MEASURE",), ("DFT1", "FFT1"), ("UL1", "UL2", "UL3", "UL1-UL2", "UL3-UL2", "UL3-UL1", "UAUX", "IL1", "IL2", "IL3", "IL1-IL2", "IL3-IL2", "IL3-IL1", "IAUX"))]:
                    send(",".join([str(random.random()) for i in range((20 + 1) * 2)]) + "\n")
                elif "?" in command:
                    send("ResponseToUnknownQuery\n")


if __name__ == "__main__":
    instrument = ScpiInstrumentStub("localhost", 1234)
    instrument.run()
