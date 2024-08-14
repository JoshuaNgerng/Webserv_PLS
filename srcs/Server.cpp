/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:55:53 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/14 08:53:12 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void ) { }

Server::Server( const Server &src ) :
	socket_fds(src.socket_fds), server_info(src.server_info) { }

Server::~Server( void ) { }

Server&	Server::operator=( const Server &src )
{
	if (this == &src)
		return (*this);
	socket_fds = src.socket_fds;
	server_info = src.server_info;
	return (*this);
}

void	Server::setUpServer( int back_log )
{
	(void)back_log;
	pollfd_t	socket_fd;
	socket_fd.events = POLLIN;
	for (std::vector<ServerBlock>::iterator it = server_info.begin();
		 it != server_info.end(); it ++)
	{
		socket_fd.fd = 
	}
}

void	Server::getNewConnection( void )
{
	for (std::vector<pollfd_t>::iterator it = server_fd.begin(); it != server_fd.end(); it ++)
	{
		;
	}
}

// void	Server::receiveRequest( int client_fd )
// {

// }

// void	Server::sentReponse( int client_fd )
// {

// }

void	Server::startupServer( void )
{
	while (1)
		getNewConnection();
}

std::vector<ServerBlock>&	Server::parseServerInfo( void ) { return (server_info); }
