/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:55:53 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/07 14:55:19 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void ) { }

Server::Server( const Server &src ) :
	server_fd(src.server_fd), server_block(src.server_block) { }

Server::~Server( void ) { }

Server&	Server::operator=( const Server &src )
{
	if (this == &src)
		return (*this);
	server_fd = src.server_fd;
	server_block = src.server_block;
	return (*this);
}

void	Server::setUpServer( const Parse &ref, int back_log )
{
	// pollfd_t	fd = {.events = POLLIN, .revents = 0};
	// sockaddr_t	addr = {.sin_family = domain, .sin_port = 0, .sin_addr.s_addr = s_addr, .sin_zero = {0}};

	// addr.sin_port = htons(ref.getPort());
	// fd.fd = socket(domain, service, protocol);
	// if (fd.fd < 0)
	// 	return ; // cannot establish connection
	// if (bind(fd.fd, (sockaddr *)&addr, sizeof(addr)) < 0)
	// 	return ;
	// 	// throw TcpError("Cannot bind socket");
	// if (listen(fd.fd, back_log) < 0)
	// 	return ;
	// 	// throw TcpError("Cannot listening to socket");
	// server_fd.push_back(fd);
	// // server_block.push_back();
}

void	Server::getNewConnection( void )
{
	for (std::vector<pollfd_t>::iterator it = server_fd.begin(); it != server_fd.end(); it ++)
	{
		;
	}
}

void	Server::receiveRequest( int client_fd, const ServerMethod &ref )
{

}

void	Server::sentReponse( int client_fd, const ServerMethod &ref )
{

}

void	Server::startupServer( void )
{
	while (1)
		getNewConnection();
}

void	Server::startupServer( const Parse &ref )
{
	// setUpServer();
	startupServer();
}
