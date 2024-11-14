#!/usr/bin/env python3
import cgi
import json
import sys

COURSES_FILE = '../storage/courses.db'

def load_courses():
    with open(COURSES_FILE, 'r') as f:
        courses_data = f.read().strip().split("\n\n")

    courses = []
    for course_data in courses_data:
        # print(course_data, file=sys.stderr)
        course_info = course_data.split("\n")
        course = {}
        # print(course_info, file=sys.stderr)
        for line in course_info:
            if line == "":
                continue 
            key, value = line.split(":", 1)
            course[key.strip()] = value.strip()
        courses.append(course)

    return courses


print("Content-Type: application/json", end="\r\n\r\n")

courses = load_courses()

print(json.dumps(courses))