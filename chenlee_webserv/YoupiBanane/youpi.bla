#!/usr/bin/env python
import sys

def main():
    # Print HTTP headers
    # print("Status: 200 OK")
    # print("Content-Type: text/html; charset=utf-8\n")

    # Read from standard input (sys.stdin) which is how CGI receives data
    input_data = sys.stdin.read()
    
    # Convert the input data to uppercase
    output_data = input_data.upper()
    
    # Output the transformed data
    print(output_data, end='', flush=True)

if __name__ == "__main__":
    main()