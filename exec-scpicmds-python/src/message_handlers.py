from typing import Optional
from abc import ABC, abstractmethod
import socket


class IMessageHandler(ABC):
    @abstractmethod
    def send_message(self, message: str) -> None:
        pass

    @abstractmethod
    def receive_response(self) -> Optional[str]:
        pass


class TCPHandler(IMessageHandler):
    def __init__(self, ip_address: str, port_number: int, receive_timeout: Optional[int] = None) -> None:
        self._ip_address = ip_address
        self._port_number = port_number
        self._receive_timeout = receive_timeout
        self._receive_buffer = ""
        self._responses = list()
        self._socket = None
        self._connect()
    
    def __del__(self) -> None:
        if self.connected:
            self._disconnect()

    @property
    def connected(self) -> bool:
        return self._socket is not None

    def send_message(self, message: str) -> None:
        if self._socket is not None:
            self._socket.send(message.encode())

    def receive_response(self) -> Optional[str]:
        if len(self._responses) == 0:
            try:
                chunk = self._socket.recv(2048).decode()
            except TimeoutError:
                return None
            self._receive_buffer += chunk
            # Split up complete responses and add them to the result list
            # There might also be some results from further queries of the current message
            while "\n" in self._receive_buffer:
                idx = self._receive_buffer.index("\n")
                answer = self._receive_buffer[:idx]
                self._responses.append(answer)
                self._receive_buffer = self._receive_buffer[idx + 1:]
        return self._responses.pop(0) if len(self._responses) > 0 else None

    def _connect(self) -> bool:
        if not self.connected:
            self._socket = socket.socket()
            if self._receive_timeout is not None and self._receive_timeout >= 0:
                self._socket.settimeout(self._receive_timeout / 1000)
            try:
                self._socket.connect((self._ip_address, self._port_number))
                return True
            except Exception as e:
                if self._socket is not None:
                    self._socket.close()
                    self._socket = None
                return False
        else:
            return False

    def _disconnect(self) -> bool:
        if self.connected:
            self._socket.shutdown(socket.SHUT_RDWR)
            self._socket.close()
            self._socket = None
            return True
        else:
            return False
