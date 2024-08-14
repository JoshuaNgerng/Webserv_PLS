/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:55:53 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/14 17:49:44 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void ) { }

// Server::Server( const Server &src ) :
// 	server_no(src.server_no), client_limit(src.client_limit),
// 	server_fds(src.server_fds), client_fds(src.client_fds),
// 	server_info(src.server_info) { }

Server::~Server( void ) { }

// Server&	Server::operator=( const Server &src )
// {
// 	if (this == &src)
// 		return (*this);
// 	server_no = src.server_no;
// 	client_limit = src.client_limit;
// 	server_fds = src.server_fds;
// 	client_fds = src.client_fds;
// 	server_info = src.server_info;
// 	return (*this);
// }

void	Server::setupSocketfds( void ) {
	typedef std::vector<ServerBlock>::iterator iter;
	server_no = 0;
	for (iter it = server_info.begin(); it != server_info.end(); it ++) {
		server_no += it->listen.size();
	}
	server_index = server_no;
	long	upper_limit = (server_no + 1) * backlog_limit + server_no;
	if (upper_limit > UINT_MAX)
		server_limit = UINT_MAX;
	else
		server_limit = upper_limit;
	pollfd_t	socket_fd;
	socket_fd.fd = -1;
	socket_fd.events = POLLIN;
	socket_fds.insert(socket_fds.end(), server_limit, socket_fd);
}

void	Server::setupServer( void ) {
	typedef std::vector<ServerBlock>::iterator ServerIter;
	typedef std::vector<Socket>::iterator SocketIter;
	size_t	server_index = 0;
	size_t	serverblock_index = 0;
	for (ServerIter it = server_info.begin(); it != server_info.end(); it ++) {
		for (SocketIter addr_it = it->listen.begin(); addr_it != it->listen.end(); addr_it ++) {
			const sockaddr_t	&addr = addr_it->refAddress();
			socket_fds[server_index].fd = socket(addr.sin_family, socket_type, socket_protocol);
			if (socket_fds[server_index].fd < 0)
				return ;
			if (bind(socket_fds[server_index].fd, (sockaddr *)&addr, socklen) < 0)
				return ;
			if (listen(socket_fds[server_index].fd, backlog_limit) < 0)
				return ;
			server_mapping[server_index] = serverblock_index;
			server_index ++;
		}
		serverblock_index ++;
	}
}

void	Server::getNewConnection( int server_fd ) {
	if (server_index == server_limit) {
		std::cout << "max connection capacity met\n";
		return ;
	}
	uint32_t	index = findAvaliableSlot();
	socklen_t	temp = len;
	sockaddr_t	temp;
	socket_fds[index].fd = accept(server_fd, (sockaddr_t *)&temp , &temp);
	if (socket_fds[index].fd < 0)
		return ; // error
	server_index ++;
}

// void	Server::receiveRequest( int client_fd )
// {

// }

// void	Server::sentReponse( int client_fd )
// {

// }

void	Server::loopServer( void ) {
	if (poll(getSocketfds(), server_limit, timeout) < 0)
		return ; // throw error?
	for (size_t index = 0; index != server_limit; index ++) {
		int	ref_fd = socket_fds[index].fd;
		if (ref_fd < 0) {
			continue ;
		}
		if (index < server_no) {
			getNewConnection(ref_fd);
			continue ;
		}
		
	}
}

void	Server::startServerLoop( void )
{
	setupSocketfds();
	setupServer();
	while (1) {
		loopServer();
	}
}

uint32_t	Server::findAvaliableSlot( void ) const {
	typedef std::vector<pollfd_t>::const_iterator iter;
	uint32_t	index = 0;
	for (iter it = socket_fds.begin(); it != socket_fds.end(); it ++) {
		if (it->fd == -1)
			return (index);
		index ++;
	}
	return (server_limit);
} 

std::vector<ServerBlock>&	Server::parseServerInfo( void ) { return (server_info); }

pollfd_t*	Server::getSocketfds( void ) { return (&(*(socket_fds.begin()))); }
