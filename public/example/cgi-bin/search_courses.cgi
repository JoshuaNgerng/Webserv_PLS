#!/usr/bin/env python3
import cgi
import json
import urllib.parse

COURSES_FILE = '../storage/courses.db'

def load_courses():
    with open(COURSES_FILE, 'r') as f:
        courses = []
        current_course = {}
        for line in f:
            line = line.strip()
            if line.startswith('Title:'):
                if current_course:
                    courses.append(current_course)
                current_course = {'title': line[7:], 'subtitle': '', 'description': '', 'image': ''}
            elif line.startswith('Subtitle:'):
                current_course['subtitle'] = line[10:]
            elif line.startswith('Description:'):
                current_course['description'] = line[12:]
            elif line.startswith('Image:'):
                current_course['image'] = line[7:]
        if current_course:
            courses.append(current_course)
    return courses

def search_courses(query):
    courses = load_courses()

    if not query:
        return courses
    query = query.lower()
    matching_courses = []

    for course in courses:
        print(course['title'].lower())
        if query in course['title'].lower() or query in course['subtitle'].lower():
            matching_courses.append(course)

    return matching_courses

form = cgi.FieldStorage()
query = form.getvalue('query', '').strip()

matching_courses = search_courses(query)

print("Content-Type: application/json", end="\r\n\r\n")

print(json.dumps(matching_courses))