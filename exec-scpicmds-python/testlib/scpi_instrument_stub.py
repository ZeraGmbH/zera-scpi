import sys
sys.path.insert(0, '.')
import socket
import random
from testlib.tcp_server_helper import VerboseTcpServer


# This class only handles a subset of the commands and of the syntax (e.g. queries without parameters) and returns pseudo responses
class ScpiInstrumentStub(VerboseTcpServer):
    def __init__(self, ip_address: str, port_number: int) -> None:
        VerboseTcpServer.__init__(self, ip_address, port_number)

    def on_client_receive_message(self, client: socket.socket, message: str) -> None:
        super().on_client_receive_message(client, message)
        send = lambda response: client.send(response.encode())
        commands = [command.strip().upper() for command in message.split("|")]
        for command in commands:
            if command.endswith("?"):
                keywords = command.replace("?", "").split(":")
                match keywords:
                    case ["*IDN"]:
                        send("ZERA SCPI Instrument Stub version 0.0.1\n")
                    case ["*STB"]:
                        send("+0\n")
                    case ["MEASURE", ("DFT1" | "FFT1"), ("UL1" | "UL2" | "UL3" | "UL1-UL2" | "UL3-UL2" | "UL3-UL1" | "UAUX" | "IL1" | "IL2" | "IL3" | "IL1-IL2" | "IL3-IL2" | "IL3-IL1" | "IAUX")]:
                        send(",".join([str(random.random()) for i in range((20 + 1) * 2)]) + "\n")
                    case _:
                        send("ResponseToUnknownQuery\n")


if __name__ == "__main__":
    instrument = ScpiInstrumentStub("localhost", 1234)
    instrument.run()
