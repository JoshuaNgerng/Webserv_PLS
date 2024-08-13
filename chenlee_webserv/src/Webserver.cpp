/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 16:06:07 by jatan             #+#    #+#             */
/*   Updated: 2024/08/10 21:12:52 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/**
 * @brief Constructor for Webserver class.
 *
 * @param port The port number to listen on.
 * @param hostname The hostname to bind to.
 * @param server The name of the server.
 */
Webserver::Webserver(std::vector<ServerBlock *> &serverBlocks) : _logger(Logger("Webserver"))
{

	for (std::vector<ServerBlock *>::iterator it = serverBlocks.begin(); it != serverBlocks.end(); it++)
	{
		setupServerSocket(**it);
	}
}

Webserver::~Webserver()
{
	for (std::vector<Connection *>::iterator it = _connections.begin(); it != _connections.end(); it++)
	{
		delete *it;
	}
	_connections.clear();
	for (std::map<int, ServerBlock *>::iterator it = _serverBlocks.begin(); it != _serverBlocks.end(); it++)
	{
		delete it->second;
	}
	_serverBlocks.clear();
	this->_logger.log("Webserver cleaned up");
}

void Webserver::start()
{
	while (1)
	{
		configureSelect();
		int activity = select(_maxFd + 1, &_readFds, &_writeFds, NULL, NULL);

		if (activity < 0)
		{
			perror("select");
			exit(1);
		}
		handleConnections();
	}
}

void Webserver::setupServerSocket(ServerBlock &serverBlock)
{
	int server_socket;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("socket");
		exit(1);
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(serverBlock.getListen());
	address.sin_addr.s_addr = INADDR_ANY;

	int opt = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(1);
	}
	if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind");
		exit(1);
	}

	// socketfd for accepting incoming connections, and backlog for queueing incoming connections
	if (listen(server_socket, 100) < 0)
	{
		perror("listen");
		exit(1);
	}

	setNonBlocking(server_socket);
	// std::cout << serverBlock.listen << std::endl;
	_serverBlocks[server_socket] = &serverBlock;
}

void Webserver::setNonBlocking(int sock_fd)
{
	int opts = fcntl(sock_fd, F_GETFL);
	if (opts < 0)
	{
		perror("fcntl(F_GETFL)");
		exit(1);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(sock_fd, F_SETFL, opts) < 0)
	{
		perror("fcntl(F_SETFL)");
		exit(1);
	}
}

void Webserver::configureSelect(void)
{
	FD_ZERO(&_readFds);
	FD_ZERO(&_writeFds);
	_maxFd = 0;

	for (std::map<int, ServerBlock *>::iterator it = _serverBlocks.begin(); it != _serverBlocks.end(); it++)
	{
		FD_SET(it->first, &_readFds);
		if (it->first > _maxFd)
		{
			_maxFd = it->first;
		}
	}

	for (std::vector<Connection *>::iterator it = _connections.begin(); it != _connections.end(); it++)
	{
		FD_SET((*it)->fd, &_readFds);
		if ((*it)->hasResponse())
		{
			FD_SET((*it)->fd, &_writeFds);
		}
		if ((*it)->fd > _maxFd)
		{
			_maxFd = (*it)->fd;
		}
	}
}

void Webserver::handleConnections()
{
	for (std::map<int, ServerBlock *>::iterator it = _serverBlocks.begin(); it != _serverBlocks.end(); it++)
	{
		if (FD_ISSET(it->first, &_readFds))
		{
			this->_logger.info("Receive read from server");
			acceptNewConnection(it->first, it->second);
		}
	}

	for (std::vector<Connection *>::iterator it = _connections.begin(); it != _connections.end();)
	{
		int fd = (*it)->fd;
		if (FD_ISSET(fd, &_readFds))
		{
			// this->_logger.log("Something from read fds...");
			if ((*it)->readData() == false)
			{
				_logger.info("close connection after read failed");
				// close_conn = true;
				close(fd);
				FD_CLR(fd, &_readFds);
				FD_CLR(fd, &_writeFds);
				delete *it;
				it = _connections.erase(it);
				continue;
			}
		}

		if (FD_ISSET(fd, &_writeFds))
		{
			if ((*it)->sendData())
			{
				// !BUG tester failed when ran the second time
				// FATAL ERROR ON LAST TEST: read tcp 127.0.0.1:49868->127.0.0.1:80:
				_logger.info("close connection after send");
				delete *it;
				it = _connections.erase(it);
				close(fd);
				FD_CLR(fd, &_readFds);
				FD_CLR(fd, &_writeFds);
				continue;
			}
		}
		++it;
	}
}

void Webserver::acceptNewConnection(int server_socket, ServerBlock *serverBlock)
{
	struct sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);
	int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
	if (client_socket < 0)
	{
		perror("accept");
	}
	else
	{
		setNonBlocking(client_socket);
		_connections.push_back(new Connection(client_socket, serverBlock));
	}
}
