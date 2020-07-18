# -*- coding: utf-8 -*-
__author__ = '0xCCCCCCCC'

from socket import *

SERVER_NAME = '127.0.0.1'
SERVER_PORT = 9876


def main():
    buffer_size = 2048
    client_socket = socket(AF_INET, SOCK_DGRAM)
    while True:
        message = input('Please input a sentence:')
        if message == 'exit':
            break
        client_socket.sendto(message.encode('utf-8'), (SERVER_NAME, SERVER_PORT))
        modified_msg, server_addr = client_socket.recvfrom(buffer_size)
        print('Received reply from server({0})'.format(server_addr))
        print('-> ' + str(modified_msg))

    client_socket.close()

if __name__ == '__main__':
    main()
