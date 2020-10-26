# -*- coding: utf-8 -*-
__author__ = '0xCCCCCCCC'

import sys
import time

from socket import *

def main():
    if len(sys.argv) < 3:
        print('Usage: tcp_client.py ip port')
        return

    ip = sys.argv[1]
    port = int(sys.argv[2])

    client_socket = socket(AF_INET, SOCK_STREAM)
    client_socket.connect((ip, port))

    cnt_sent = client_socket.send('abcdefghijklmnqoprstuvwxyz'.encode('utf-8'))
    print('client sent {} bytes'.format(cnt_sent))
    time.sleep(5)
    print('awaked. send some data again')
    client_socket.send('1'.encode('utf-8'))
    client_socket.close()

if __name__ == '__main__':
    main()
