#!/usr/bin/env python3
import os
import cgi
import time
import json
import urllib.parse
import re

COURSES_FILE = '../storage/courses.db'


def fix_newlines(file_path):
    with open(file_path, 'r') as file:
        # Read the entire file content
        content = file.read()

    # Case 1: If the file only contains newlines (and nothing else)
    if content.strip() == '':
        content = ''  # Remove all content, leaving the file empty

    # Case 2: Replace multiple consecutive newlines with two newlines
    else:
        content = re.sub(r'\n{3,}', '\n\n', content)

        # Ensure that there are exactly two newlines at the end of the content, if there are any newlines
        if content.endswith('\n'):
            content += '\n'

    # Write the modified content back to the file
    with open(file_path, 'w') as file:
        file.write(content)

# Usage example:
# fix_newlines('your_file.txt')




def sanitize_input(input_string):
    """Sanitize input to prevent security issues like directory traversal."""
    sanitized = input_string.strip()
    sanitized = sanitized.replace("/", "")  # Remove slashes for security
    return sanitized

def delete_course(course_title):
    """Delete the course entry based on the provided title."""
    if not os.path.isfile(COURSES_FILE):
        return {"success": False, "error": f"File '{COURSES_FILE}' not found."}

    try:
        with open(COURSES_FILE, 'r') as f:
            courses = f.readlines()
    except IOError as e:
        return {"success": False, "error": f"Error reading file: {str(e)}"}

    new_courses = []
    skip = False
    course_found = False
    already_removed = False

    for line in courses:
        if not already_removed and line.startswith(f"Title: {course_title}\n"):
            skip = True  # Skip this course's lines
            course_found = True
        elif skip and line.startswith("Image:"):
            skip = False  # End of course entry
            already_removed = True
        elif not skip:
            new_courses.append(line)

    if not course_found:
        return {"success": False, "error": "Course not found."}

    try:
        with open(COURSES_FILE, 'w') as f:
            f.writelines(new_courses)
    except IOError as e:
        return {"success": False, "error": f"Error writing to file: {str(e)}"}

    return {"success": True}

# Print the JSON header
print("Content-Type: application/json", end="\r\n\r\n")

# Manually parse the QUERY_STRING
query_string = os.environ.get('QUERY_STRING', '')
params = urllib.parse.parse_qs(query_string)

# Get the course-title from the query string
course_title = params.get('course-title', [None])[0]

if course_title:
    sanitized_title = sanitize_input(course_title)
    response = delete_course(sanitized_title)
else:
    response = {"success": False, "error": "Course title not provided in query string."}

# Output the JSON response
print(json.dumps(response))


fix_newlines("../storage/courses.db")
time.sleep(0.01)