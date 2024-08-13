/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.4ss2kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:45:54 by jatan             #+#    #+#             */
/*   Updated: 2024/05/16 21:19:42 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_BLOCK_HPP
#define SERVER_BLOCK_HPP

#include <iostream>
#include <string>
#include <sstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>	// struct addrinfo, gai_strerror
#include <unistd.h> // execve, dup, dup2, pipe, fork

#include "Logger.hpp"
#include "Router.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

class Router;
class ServerBlock
{
public:
	int listen;
	string hostname;
	Router router;
    int bodyLimit;
    IntStringMap _errorPages;
    string _root;
    std::string _projectDir;

	ServerBlock();
	ServerBlock(Router router);

    void display() const;
    int getPort() const;
    string getServerName() const;
    int getBodyLimit() const;
    IntStringMap& getErrorPages();
    Router& getRouter();
    std::string getRoot() const;
    int getListen() const;
    std::string getProjectDir() const;

    void setListen(int listen);
    void setBodyLimit(int bodyLimit);
    void addErrorPage(int code, string path);
    void setRoot(string root);
	string getHostname() const;


    string handleRequest(string request);

};

#endif /* ****************************************************** SERVER_BLOCK_HPP */
