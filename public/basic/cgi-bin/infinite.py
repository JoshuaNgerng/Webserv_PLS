#!/usr/bin/python3

# print("Content-Length: 100", end="\r\n")
# print("Content-Type: text/plain", end="\r\n\r\n")

counter = 0
lim = 10000

while (True):
    if counter % 50:
        print(counter)
    counter += 1
