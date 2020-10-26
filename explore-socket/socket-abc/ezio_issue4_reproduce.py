# -*- coding: utf-8 -*-

import hashlib
import socket
import struct

SERVER_NAME = 'localhost'
SERVER_PORT = 9876

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((SERVER_NAME, SERVER_PORT))

    msg_len = 1600
    payload = 'a' * msg_len
    msg = struct.pack('!Q', msg_len) + payload.encode('utf-8')

    client_socket.sendall(msg)
    print('data sent')

    pin = client_socket.recv(256)
    print(len(pin))
    print('received hash: \t' + pin.decode('utf-8'))

    md5 = hashlib.md5()
    md5.update(payload.encode('utf-8'))
    print('local hash: \t' + md5.hexdigest())

if __name__ == '__main__':
    main()
