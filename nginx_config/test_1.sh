#!/bin/bash

# Stop nginx:
sudo systemctl stop nginx

# Check for syntax error:
sudo nginx -t -c /mnt/c/Users/alexl/42KL/Webserv/nginx_config/test_1.conf

# Set nginx configuration file path:
sudo nginx -c /mnt/c/Users/alexl/42KL/Webserv/nginx_config/test_1.conf

# Reload
sudo nginx -s reload

#Stop nginx:
# sudo nginx -s stop
