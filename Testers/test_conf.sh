#!/bin/bash
clear
echo "Test configuration script"
echo "Invalid listen format test:"
echo
echo "Test listen_1.conf"
./webserv Testers/conf_test/listen_1.conf
echo
echo "Test listen_2.conf"
./webserv Testers/conf_test/listen_2.conf
echo
echo "Test listen_3.conf"
./webserv Testers/conf_test/listen_3.conf
echo
echo "Test listen_4.conf"
./webserv Testers/conf_test/listen_4.conf


echo "Invalid index format test:"
echo
echo "Test index_1.conf"
./webserv Testers/conf_test/index_1.conf
echo
echo "Test index_2.conf"
./webserv Testers/conf_test/index_2.conf
echo
echo "Test index_3.conf"
./webserv Testers/conf_test/index_3.conf
echo