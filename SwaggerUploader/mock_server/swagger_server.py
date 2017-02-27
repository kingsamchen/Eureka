#!/usr/bin/python
# 0xCCCCCCCC

import json

from flask import Flask, request, Response, url_for

app = Flask('__name__')


@app.route('/')
def root():
    return 'Welcome and see the instruction manual as follows'


@app.route('/preupload')
def prepare_upload():
    profile = request.args['profile']
    mid = request.args['mid']
    print('profile = {0}\nmid = {1}'.format(profile, mid))

    upload_info = {
        'url': 'http://127.0.0.1:5000/upload',
        'complete': 'http://127.0.0.1:5000/complete'
    }

    return Response(json.dumps(upload_info), status=200, mimetype='application/json')


@app.route('/upload')
def upload():
    pass


@app.route('/complete')
def complete_upload():
    pass


def main():
    app.run()

if __name__ == '__main__':
    main()
