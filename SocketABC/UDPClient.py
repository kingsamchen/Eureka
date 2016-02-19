# -*- coding: utf-8 -*-
__author__ = '0xCCCCCCCC'

from socket import *

SERVER_NAME = '127.0.0.1'
SERVER_PORT = 12345


def main():
    buffer_size = 2048
    client_socket = socket(AF_INET, SOCK_DGRAM)
    message = raw_input('Please input a sentence')
    client_socket.sendto(message, (SERVER_NAME, SERVER_PORT))
    modified_msg, server_addr = client_socket.recvfrom(buffer_size)
    client_socket.close()
    print('-> ' + modified_msg)


if __name__ == '__main__':
    main()
