# -*- coding: utf-8 -*-
__author__ = '0xCCCCCCCC'

from socket import *

SERVER_NAME = '127.0.0.1'
SERVER_PORT = 12345


def main():
    buffer_size = 2048
    while True:
        message = raw_input('Please input a message:')
        if message == 'exit':
            break
        client_socket = socket(AF_INET, SOCK_STREAM)
        client_socket.connect((SERVER_NAME, SERVER_PORT))
        client_socket.send(message)
        modified_message = client_socket.recv(buffer_size)
        print('received reply: ' + modified_message)
        client_socket.close()


if __name__ == '__main__':
    main()
