#!/usr/bin/env python3

import os
import sys
import cgi
import cgitb
cgitb.enable()

print("Content-Type: text/html\n")
print("<html><body>")
print("<h1>Hello from Python CGI!</h1>")

content_length = os.environ.get('CONTENT_LENGTH')
if content_length:
    length = int(content_length)
    body = sys.stdin.read(length)
    print(f"<p>Body: {body}</p>")

print("</body></html>")
