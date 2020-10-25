# -*- coding: utf-8 -*-
# 0xCCCCCCCC

import sys

from socket import *

def main():
    if len(sys.argv) < 2:
        print('Usage: tcp_server.py [port]')
        return

    port = int(sys.argv[1])

    server_socket = socket(AF_INET, SOCK_STREAM)
    server_socket.bind(('', port))
    server_socket.listen(5)
    print('TCP server is listening at: ' + str(server_socket.getsockname()))

    while True:
        conn_socket, addr = server_socket.accept()
        print('accept an connection at: ' + str(addr))
        byte = conn_socket.recv(1)
        print('received a byte: ' + str(byte))
        conn_socket.close()
        print('the connection is closed by the server')

if __name__ == '__main__':
    main()
