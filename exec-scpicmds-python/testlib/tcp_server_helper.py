import time
import socket
import logging
import threading
from dataclasses import dataclass


@dataclass
class ClientData:
    socket: socket.socket
    id: int
    thread: threading.Thread


class TcpServer():
    def __init__(self, ip_address: str, port_number: int, listen_backlog: int = 0):
        self._ip_address = ip_address
        self._port_number = port_number
        self._socket = socket.socket()
        self._clients: list[ClientData] = list()
        self._next_client_id = 0
        self._quit = False
        self._socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self._socket.setblocking(False)  # Allows for quitting the server while in the accept-loop
        self._socket.bind((self._ip_address, self._port_number))
        self._socket.listen(listen_backlog)
    
    def __del__(self):
        self.quit()

    def run(self, run_in_background: bool = False):
        if run_in_background:
            t = threading.Thread(target=self._run)
            t.start()
        else:
            self._run()
    
    def _run(self):
        self.on_server_start()
        try:
            self._accept_clients()
        except Exception as ex:
            logging.error(ex)
        finally:
            self.on_server_stop()
            for client_data in self._clients:
                client_data.socket.close()
            self._socket.close()
    
    def quit(self):
        self._quit = True
        for client_data in self._clients:
            client_data.thread.join()
            logging.debug(f"Joined thread #{client_data.id}")


    def _accept_clients(self):
        while not self._quit:
            try:
                client_socket, _address = self._socket.accept()
            except:  # Timeout
                pass
            else:
                thread = threading.Thread(target=self.receive, args=(client_socket,))
                self._clients.append(ClientData(client_socket, self._next_client_id, thread))
                self._next_client_id += 1
                self.on_client_connection_open(client_socket)
                thread.start()
            time.sleep(0.01)  # Throttle loop as accept is non-blocking

    def receive(self, client):
        while client.fileno() >= 0:  # Loop while socket is open
            data = client.recv(2048)
            if data is None or data == b"":
                break
            self.on_client_receive_message(client, data)
        for c, client_data in enumerate(self._clients):
            if client_data.socket is client:
                del self._clients[c]
                break
        self.on_client_connection_close(client)
        client.close()

    def broadcast(self, message):
        for client_data in self._clients:
            client = client_data.socket
            if client.fileno() != -1:  # Socket already closed
                client.send(message)

    def on_server_start(self):
        pass

    def on_server_stop(self):
        pass

    def on_client_connection_open(self, client):
        pass

    def on_client_receive_message(self, client, message):
        pass

    def on_client_connection_close(self, client):
        pass


class TcpTestServer(TcpServer):
    def __init__(self, ip_address: str, port_number: int):
        TcpServer.__init__(self, ip_address, port_number)

    def on_server_start(self):
        logging.debug("Server started.")

    def on_server_stop(self):
        logging.debug("Server closed.")

    def on_client_receive_message(self, _client, message):
        logging.debug(f"Client sent message: {message}")
        self.broadcast(message)  # Sending message to all clients

    def on_client_connection_open(self, _client):
        logging.debug("Client connected.")

    def on_client_connection_close(self, _client):
        logging.debug("Client disconnected.")
