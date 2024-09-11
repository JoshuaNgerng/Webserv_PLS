#!/usr/bin/python3
import os

print("Content-Type: text/html")
print()  # Blank line to separate headers from content
print("<html>")
print("<body>")
print("<h1>Hello from CGI!</h1>")
print(f"<p>PATH_INFO: {os.environ.get('PATH_INFO')}</p>")
print("</body>")
print("</html>")
