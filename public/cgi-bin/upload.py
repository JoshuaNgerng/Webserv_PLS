#!/usr/bin/python3

import os
import cgi
import cgitb
import sys
from pathlib import Path

print("testing cgi script start", file=sys.stderr)

# Enable debugging
cgitb.enable()

# Directory to save uploaded files
UPLOAD_DIR = "list" 

# Ensure the upload directory exists
Path(UPLOAD_DIR).mkdir(parents=True, exist_ok=True)

print("Content-Type: text/html\n")  # Header to indicate an HTML response
print("<html><body>")

# Get the form data
form = cgi.FieldStorage()

# Check if a file was uploaded
if "file" not in form or not form["file"].filename:
    print("<p>No file uploaded</p>")
else:
    file_item = form["file"]
    filename = os.path.basename(file_item.filename)
    save_path = os.path.join(UPLOAD_DIR, filename)

    # Write the uploaded file to the specified directory
    try:
        with open(save_path, "wb") as output_file:
            output_file.write(file_item.file.read())
        print(f"<p>File '{filename}' uploaded successfully!</p>")
    except IOError as e:
        print(f"<p>Error saving file: {e}</p>")

print("</body></html>")
