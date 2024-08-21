/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:02:07 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/21 18:11:09 by jngerng          ###   ########.fr       */
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
	fd_counter = server_no;
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
	size_t	index = 0;
	for (ServerIter it = server_info.begin(); it != server_info.end(); it ++) {
		std::cout << "huh"<< it->listen.size() << "\n";
		for (SocketIter addr_it = it->listen.begin(); addr_it != it->listen.end(); addr_it ++) {
			const sockaddr_in_t	&addr = addr_it->refAddress();
			socket_fds[index].fd = socket(addr.sin_family, socket_type, socket_protocol);
			std::cout << "check server fd: " << socket_fds[index].fd;
			if (socket_fds[fd_counter].fd < 0) {
				std::cout << "socket failed\n";
				return ;
			}
			//flagss |= O_NONBLOCK; fcntl
			if (bind(socket_fds[index].fd, (sockaddr *)&addr, socklen) < 0) {
				std::cout << "bind failed\n";
				return ;
			}
			if (listen(socket_fds[index].fd, backlog_limit) < 0) {
				std::cout << "listen failed\n";
				return ;
			}
			server_mapping.push_back(std::make_pair(socket_fds[index].fd, it));
			index ++;
		}
	}
}

void	Server::loopServer( void ) {
	
}

void	Server::startServerLoop( int *signal ) {
	setupSocketfds();
	setupServer();
	if (!server_no) {
		std::cout << "no servers lulz\n";
		return ;
	}
	while (*signal) {
		loopServer();
	}
}

void	Server::startServerLoop( void ) {
	int	fix = 1;
	startServerLoop(&fix);
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

void	Server::addServerBlock( ServerBlock &ref ) {
	server_info.push_back(ref);
}

pollfd_t*	Server::getSocketfds( void ) {
	return (&(*(socket_fds.begin())));
}

//getters
socklen_t	Server::getSocklen( void ) const {
	return (this->socklen);
}

int	Server::getSocketType( void ) const {
	return (this->socket_type);
}

int	Server::getSocketProtocol( void ) const {
	return (this->socket_protocol);
}

int	Server::getBacklogLimit( void ) const {
	return (this->backlog_limit);
}

int	Server::getRecvFlag( void ) const {
	return (this->recv_flag);
}

int	Server::getSendFlag( void ) const {
	return (this->send_flag);
}

int	Server::getTimeout( void ) const {
	return (this->timeout);
}

int	Server::getBufferLimit( void ) const {
	return (this->buffer_limit);
}

uint32_t	Server::getServerNo( void ) const {
	return (this->server_no);
}

nfds_t	Server::getServerLimit( void ) const {
	return (this->server_limit);
}

uint32_t	Server::getFdCounter( void ) const {
	return (this->fd_counter);
}
// end of getters

//display
std::ostream&	Server::displaySocketFds( std::ostream &o ) const {
	return (displayContainer(o, socket_fds));
}

std::ostream&	Server::displayServerInfo( std::ostream &o ) const {
	return (displayContainer(o, server_info, "\n"));
}

std::ostream&	Server::displayClientInfo( std::ostream &o ) const {
	return (displayContainer(o, client_info));
}

//end of display

std::ostream&	operator<<( std::ostream &o, const pollfd_t &ref ) {
	o << "fd: " << ref.fd;
	return (o);
}

std::ostream&	operator<<( std::ostream &o, const Server& ref ) {
	o << "Server Status\n";
	o << "Const info: " << "backlog limit: " << ref.getBacklogLimit() <<
		", recv flag: " << ref.getRecvFlag() <<
		", send flag: " << ref.getSendFlag() <<
		", poll timeout: " << ref.getTimeout() <<
		", recv buffer size: " << ref.getBufferLimit() << '\n';
	o << "Server info";
	o << "Display Sockets: ";
	ref.displaySocketFds(o) << '\n';
	o << "Servers info\n";
	ref.displayServerInfo(o) << '\n';
	return (o);
}
