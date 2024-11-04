#!/usr/bin/env python3
import cgi

print("Content-Type: text/html")  # Set the response content type
print()  # End headers

# Get form data
form = cgi.FieldStorage()
text = form.getvalue("text", "")  # Get 'text' field from POST data

# Output the capitalized text
capitalized_text = text.upper()
print(f"<html><body><h1>Capitalized Text</h1><p>{capitalized_text}</p></body></html>")
