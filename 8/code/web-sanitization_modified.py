#!/usr/bin/env python3

import sys
import nh3
try:
    from http.server import HTTPServer, BaseHTTPRequestHandler
    from urllib.parse import parse_qs
except ImportError:
    sys.exit('ERROR: It seems like you are not running Python 3. '
             'This script only works with Python 3!')


stored_messages = []

form_doc = '''
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <title>SEC Message Board</title>
    <script src="https://cdn.ckeditor.com/ckeditor5/12.2.0/classic/ckeditor.js"></script>
</head>
<body>
    <h1>SEC Message Board</h1>
{messages}
<form method="post">
    <h2>Post your own message:</h2>
    <textarea name="content" id="editor"></textarea>
    <input type="submit" value="post">
</form>
    <script>
    ClassicEditor
        .create( document.querySelector( '#editor' ), {{toolbar: ['bold','italic']}} )
        .catch( error => {{
            console.error( error );
        }} );
    </script>
</body>
</html>
'''

class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-Type', 'text/html;charset=utf-8')
        self.end_headers()

        messages = "<ul>"
        for message in stored_messages:
            messages += f"<li>{nh3.clean(message)}</li>"
        messages += "</ul>"

        self.wfile.write(bytes(form_doc.format(messages=messages), 'UTF-8'))

    def do_POST(self):
        content_length = self.headers['Content-Length']
        body = self.rfile.read(int(content_length))
        post_dict = parse_qs(str(body, 'UTF-8'))
        print(post_dict['content'][0])
        if "content" in post_dict:
            stored_messages.append(post_dict['content'][0])

        self.do_GET()


if __name__ == '__main__':
    server = HTTPServer(('localhost', 8081), MyHandler)
    print("Starting web server on http://localhost:8081/")
    server.serve_forever()
