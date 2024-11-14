#!/usr/bin/env python3
import cgi
import json
import os
import shutil

COURSES_FILE = '../storage/courses.db'
UPLOAD_DIR = '../storage/uploads'

def save_uploaded_image(fileitem):
    if fileitem.filename:
        filename = os.path.basename(fileitem.filename)
        filepath = os.path.join(UPLOAD_DIR, filename)

        with open(filepath, 'wb') as f:
            shutil.copyfileobj(fileitem.file, f)

        return f"storage/uploads/{filename}"
    return None

print("Content-Type: application/json")
print()

form = cgi.FieldStorage()

image = form['course-image'] if 'course-image' in form else None
title = form.getvalue('course-title', '')
subtitle = form.getvalue('course-subtitle', '')
description = form.getvalue('course-description', '')

image_path = save_uploaded_image(image)

new_course = f"Title: {title}\nSubtitle: {subtitle}\nDescription: {description}\nImage: {image_path}\n\n"
with open(COURSES_FILE, 'a') as f:
    f.write(new_course)

response = {"success": True}
print(json.dumps(response))
