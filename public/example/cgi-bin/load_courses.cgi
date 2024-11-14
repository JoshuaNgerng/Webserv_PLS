#!/usr/bin/env python3
import cgi
import json
import os

COURSES_FILE = './storage/courses.txt'

def load_courses():
    with open(COURSES_FILE, 'r') as f:
        courses_data = f.read().strip().split("\n\n")

    courses = []
    for course_data in courses_data:
        course_info = course_data.split("\n")
        course = {}
        for line in course_info:
            key, value = line.split(":", 1)
            course[key.strip()] = value.strip()
        courses.append(course)

    return courses


print("Content-Type: application/json")
print()

courses = load_courses()

print(json.dumps(courses))