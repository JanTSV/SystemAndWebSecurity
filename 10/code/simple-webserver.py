#!/usr/bin/env python3

import sys
try:
    from http.server import HTTPServer, BaseHTTPRequestHandler
except ImportError:
    sys.exit('ERROR: It seems like you are not running Python 3. '
             'This script only works with Python 3!')


class MyHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        self.send_response(200)
        self.send_header('Access-Control-Allow-Origin', 'https://cors.demo.sec.uni-stuttgart.de')
        self.end_headers()

        self.wfile.write("SUCCESS!".encode("utf-8"))

    def do_OPTIONS(self):
        self.send_response(200)
        self.send_header('Access-Control-Allow-Origin', 'https://cors.demo.sec.uni-stuttgart.de')
        self.send_header('Access-Control-Allow-Headers', 'x-sec')
        self.end_headers()


if __name__ == '__main__':
    server = HTTPServer(('', 8081), MyHandler)
    print ("Starting web server on http://localhost:8081/")
    server.serve_forever()
