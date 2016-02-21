# -*- coding: utf-8 -*-
__author__ = '0xCCCCCCCC'

from socket import *

SERVER_PORT = 12345


def main():
    buffer_size = 2048
    server_socket = socket(AF_INET, SOCK_STREAM)
    server_socket.bind(('', SERVER_PORT))
    server_socket.listen(1)
    print('server is online')
    while True:
        conn_socket, addr = server_socket.accept()
        message = conn_socket.recv(buffer_size)
        modified_message = message.upper()
        conn_socket.send(modified_message)
        conn_socket.close()

if __name__ == '__main__':
    main()
