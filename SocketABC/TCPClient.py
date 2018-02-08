# -*- coding: utf-8 -*-
__author__ = '0xCCCCCCCC'

from socket import *

SERVER_NAME = '127.0.0.1'
SERVER_PORT = 8088


def main():
    #buffer_size = 2048

    client_socket = socket(AF_INET, SOCK_STREAM)
    client_socket.connect((SERVER_NAME, SERVER_PORT))

    while True:
        message = input('Please input a message:')
        if message == 'exit':
            client_socket.close()
            break

        count = client_socket.send(message.encode('utf-8'))
        print('data in {} bytes sent'.format(count))
        #modified_message = client_socket.recv(buffer_size)
        #print('received reply: ' + modified_message)

if __name__ == '__main__':
    main()
