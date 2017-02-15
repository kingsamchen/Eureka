#!/usr/bin/python
# 0xCCCCCCCC

import cgi
import os
import SocketServer
import sys

from SimpleHTTPServer import SimpleHTTPRequestHandler

SAVE_DIR = ''

class ServerHandler(SimpleHTTPRequestHandler):
    def do_GET(self):
        print('--- GET Request ---')
        print(self.headers)
        SimpleHTTPRequestHandler.do_GET(self)

    def do_POST(self):
        print('--- POST Request ---')
        print(self.headers)
        form = cgi.FieldStorage(fp=self.rfile,
                                headers=self.headers,
                                environ={'REQUEST_METHOD':'POST',
                                         'CONTENT_TYPE':self.headers['Content-Type']})
        print('--- POST values ---')
        dest_filepath = os.path.join(SAVE_DIR, 'test-video')
        data = form['file'].file.read()
        with open(dest_filepath, 'ab') as df:
            df.write(data)
        current_chunk = form.getvalue('chunk')
        total_chunks = form.getvalue('chunks')
        chunk_size = form.getvalue('filesize')
        print('receive data chunk {0}/{1} with size {2}'.format(current_chunk, total_chunks,
                                                                chunk_size))
        SimpleHTTPRequestHandler.do_GET(self)


def main():
    global SAVE_DIR
    if len(sys.argv) > 3:
        SAVE_DIR = sys.argv[1]
        PORT = int(sys.argv[2])
        HOST = sys.argv[1]
    elif len(sys.argv) > 2:
        SAVE_DIR = sys.argv[1]
        PORT = int(sys.argv[1])
        HOST = ""
    elif len(sys.argv) > 1:
        SAVE_DIR = sys.argv[1]
    else:
        SAVE_DIR = "e:/Caches"
        PORT = 8000
        HOST = "127.0.0.1"

    Handler = ServerHandler
    httpd = SocketServer.TCPServer((HOST, PORT), Handler)

    print('Tiny http server for breakpad test')
    print("Serving at: http://{0}:{1}".format(HOST, PORT))

    httpd.serve_forever()


if __name__ == '__main__':
    main()
