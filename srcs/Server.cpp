/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:55:53 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/13 02:47:25 by jngerng          ###   ########.fr       */
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

void	Server::setUpServer( Parse &ref, int back_log )
{
	for (std::vector<ServerBlock>::iterator it = server_info.begin();
		 it != server_info.end(); it ++)
	{
		pollfd_t	temp = {0};
		it->listen;
	}
}

void	Server::getNewConnection( void )
{
	for (std::vector<pollfd_t>::iterator it = server_fd.begin(); it != server_fd.end(); it ++)
	{
		;
	}
}

void	Server::receiveRequest( int client_fd, const Parse &ref )
{

}

void	Server::sentReponse( int client_fd, const Parse &ref )
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
