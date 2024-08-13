/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:46:03 by jatan             #+#    #+#             */
/*   Updated: 2024/08/10 21:08:33 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>	// struct addrinfo, gai_strerror
#include <unistd.h> // execve, dup, dup2, pipe, fork
#include <fcntl.h> // fcntl
#include <sys/select.h> // select

#include "Request.hpp"
#include "Response.hpp"
#include "StaticFileHandler.hpp"
#include "Router.hpp"

#include <iostream>
#include <cstring> // strerror
#include <cerrno>  // errno
#include <sstream> // stringstream
#include <string>
#include <stdexcept>
#include <map>


#include "Logger.hpp"
#include "Connection.hpp"
#include "ServerBlock.hpp"

// #define PORT "8080"
// #define BACKLOG 10
// #define BUFFER_SIZE 1024

class Webserver
{
public:
	typedef std::map<int, ServerBlock*> serverBlockMap;
	typedef std::map<int, ServerBlock*>::iterator serverBlockMapIt;
	typedef std::vector<Connection *> connectionVec;
	typedef std::vector<Connection *>::iterator connectionVecIt;

	Webserver(std::vector<ServerBlock*>& serverBlocks);
	~Webserver();

	void start();

private:
	int createSocket(std::string port, std::string hostname);
	void waitForRequest(void);
	void handleRequest(int sock);
	// void acceptConnection(fd_set&, int& fdmax);

	void setupServerSocket(ServerBlock& serverBlock);
	void setNonBlocking(int sock_fd);
	void configureSelect(void);
	void handleConnections(void);
	void acceptNewConnection(int server_socket, ServerBlock *serverBlock);
	bool echoMessage(int client_socket);

	int _listener; 
	Logger _logger;
	fd_set _readFds, _writeFds;
	int _maxFd;
	serverBlockMap _serverBlocks;
	connectionVec _connections;

	Webserver();
};

#endif
