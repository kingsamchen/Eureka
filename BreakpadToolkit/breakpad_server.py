#!/usr/bin/python
# 0xCCCCCCCC

import cgi
import logging
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
        dump_filenames = form.getvalue('dump_file_list').split(';')
        for filename in dump_filenames:
            data = form[filename].file.read()
            dest_filepath = os.path.join(SAVE_DIR, filename)
            print('saving data at ' + dest_filepath)
            with open(dest_filepath, 'wb') as df:
                df.write(data)
            print(filename + ' is transferred')

        SimpleHTTPRequestHandler.do_GET(self)


def main():
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
        SAVE_DIR = "d:/misc/dumps"
        PORT = 8000
        HOST = ""

    Handler = ServerHandler
    httpd = SocketServer.TCPServer((HOST, PORT), Handler)

    print('Tiny http server for breakpad test')
    print("Serving at: http://{0}:{1}".format(HOST, PORT))

    httpd.serve_forever()


if __name__ == '__main__':
    main()
