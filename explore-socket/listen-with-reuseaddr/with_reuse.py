# -*- coding: utf-8 -*-
# 0xCCCCCCCC

from socket import *

import sys
import time

SERVER_PORT = 9876

# 用户检测 windows 下 SO_REUSEADDR 的效果

def main():
    buffer_size = 2048
    server_socket = socket(AF_INET, SOCK_STREAM)
    server_socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    server_socket.bind(('', SERVER_PORT))
    server_socket.listen(1)
    print('server is online')
    while True:
        try:
            time.sleep(1)
        except KeyboardInterrupt:
            sys.exit()

if __name__ == '__main__':
    main()
