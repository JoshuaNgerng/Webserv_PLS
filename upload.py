#!/usr/bin/python3

import cgi
import os
import sys
import io

# The path to the file you want to modify
file_path = '/var/www/data/textfile.txt'

# Get the HTTP method (POST, PUT, etc.)
method = os.environ.get('REQUEST_METHOD')

# Read the content length (the size of the incoming data)
content_length = int(os.environ.get('CONTENT_LENGTH', 0))

# Read the body of the request (up to the specified content length)
input_data = sys.stdin.read(content_length)

# Function to handle POST (rewrite file)
def handle_post(data):
    with open(file_path, 'w') as f:
        f.write(data)

# Function to handle PUT (append to file)
def handle_put(data):
    with open(file_path, 'a') as f:
        f.write(data)

# Handle the request based on the HTTP method
if method == 'POST':
    handle_post(input_data)
    status_code = 200
    status_message = 'OK - File Overwritten'
elif method == 'PUT':
    handle_put(input_data)
    status_code = 200
    status_message = 'OK - Data Appended'
else:
    status_code = 405
    status_message = 'Method Not Allowed'

# Output the HTTP headers
print(f"Content-type: text/plain\n")
print(f"{status_message}")

# curl -X POST --data "This is new content for the file" http://your-server/cgi-bin/update_file.py
# curl -X PUT --data "This is additional content for the file" http://your-server/cgi-bin/update_file.py
