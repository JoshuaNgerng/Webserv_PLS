/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:55:53 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/13 17:51:28 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void ) { }

Server::Server( const Server &src ) :
	server_fd(src.server_fd), server_info(src.server_info) { }

Server::~Server( void ) { }

Server&	Server::operator=( const Server &src )
{
	if (this == &src)
		return (*this);
	server_fd = src.server_fd;
	server_info = src.server_info;
	return (*this);
}

void	Server::setUpServer( int back_log )
{
	(void)back_log;
	for (std::vector<ServerBlock>::iterator it = server_info.begin();
		 it != server_info.end(); it ++)
	{
		pollfd_t	temp = {.fd = 0, .events = 0, .revents = 0};
		(void)temp;
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
