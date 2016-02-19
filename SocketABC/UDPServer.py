# -*- coding: utf-8 -*-
__author__ = '0xCCCCCCCC'

from socket import *

SERVER_PORT = 12345


def main():
    buffer_size = 2048
    server_socket = socket(AF_INET, SOCK_DGRAM)
    server_socket.bind(('', SERVER_PORT))
    print('The server is online')
    while True:
        message, client_addr = server_socket.recvfrom(buffer_size)
        print('Received message from client({0})'.format(client_addr))
        modified_message = message.upper()
        server_socket.sendto(modified_message, client_addr)
        print('Sending a reply back')

if __name__ == '__main__':
    main()
