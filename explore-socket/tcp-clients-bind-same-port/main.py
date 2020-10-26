# -*- coding: utf-8 -*-
# 0xCCCCCCCC

import socket
import sys
import time

LOCAL_PORT = 9999

def main():
    if len(sys.argv) < 3:
        print('Usage: main.py <ip> <port>')
        sys.exit(0)

    ip = sys.argv[1]
    port = int(sys.argv[2])

    cli = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    platform = sys.platform
    if platform == 'win32':
        cli.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    elif platform == 'linux':
        cli.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        #cli.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
    else:
        raise Exception('unsupported platform {}'.format(platform))

    cli.bind(('', LOCAL_PORT))
    cli.connect((ip, port))
    print('Connected! local={} peer={}'.format(cli.getsockname(), cli.getpeername()))
    while True:
        try:
            cli.sendall(b'beep')
            reply = cli.recv(255)
            print('-> {}'.format(reply.decode('utf-8')))
            time.sleep(1)
        except KeyboardInterrupt:
            print('bye')
            sys.exit()


if __name__ == '__main__':
    main()
