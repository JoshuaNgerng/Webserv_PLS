#!/usr/bin/env python3
import cgi
import json

COURSES_FILE = '../storage/courses.db'

def delete_course(course_title):
    with open(COURSES_FILE, 'r') as f:
        courses = f.readlines()

    new_courses = []
    skip = False
    for line in courses:
        if line.startswith(f"Title: {course_title}\n"):
            skip = True
        elif skip and line.startswith("Image:"):
            skip = False
        elif not skip:
            new_courses.append(line)

    with open(COURSES_FILE, 'w') as f:
        f.writelines(new_courses)

    return True

print("Content-Type: application/json")
print()

form = cgi.FieldStorage()
course_title = form.getvalue('course-title', None)

if course_title:
    delete_course(course_title)
    response = {"success": True}
else:
    response = {"success": False, "error": "Course not found."}

print(json.dumps(response))