/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:02:07 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/23 18:44:01 by joshua           ###   ########.fr       */
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

void	Server::setNonBlockFd( int fd ) {
	int flagss = fcntl(fd, F_GETFL);
	if (flagss < 0) {
		return ; // cant get flags
	}
	flagss |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flagss) < 0) {
		return ; // cant set nonblock flag
	}
}

int	Server::setListeningSocket( const sockaddr_in_t &addr, int socket_type, int socket_protocol ) {
	int	fd = socket(addr.sin_family, socket_type, socket_protocol);
	if (fd < 0) {
		return (-1); // cant set socket
	}
	if (bind(fd, (sockaddr *)&addr, socklen) < 0) {
		// std::cout << "bind failed\n";
		return ; // bind failed
	}
	if (listen(fd, backlog_limit) < 0) {
		// std::cout << "listen failed\n";
		return ; // listen failed
	}
}

void	Server::setupSocketfds( void ) {
	typedef std::vector<ServerBlock>::iterator iter;
	server_no = 0;
	for (iter it = server_info.begin(); it != server_info.end(); it ++) {
		server_no += it->listen.size();
	}
	fd_counter = server_no;
	poll_tracker = fd_counter;
	long	upper_limit = (server_no + 1) * backlog_limit + server_no;
	if (upper_limit > UINT_MAX)
		server_limit = UINT_MAX;
	else
		server_limit = upper_limit;
	poll_tracker = server_no;
	pollfd_t	socket_fd;
	socket_fd.fd = -1;
	socket_fd.events = POLLIN;
	socket_fds.insert(socket_fds.end(), server_limit, socket_fd);
	buffer_new_fd.insert(buffer_new_fd.end(), server_no * 2, socket_fd);
}

void	Server::resetFds( void ) {
	nfds_t	i = 0;
	nfds_t	buffer_index = 0;
	for (i = 0; i < poll_tracker; i ++) {
		if (socket_fds[i].fd == -1) {
			if (buffer_index < buffer_counter) {
				std::swap(buffer_new_fd[buffer_index], socket_fds[i]);
				buffer_index ++;
			}
			else {
				;	
			}
		}
	}
	for (buffer_index; buffer_index < buffer_counter; buffer_index ++) {
		while (i < server_limit)
		{
			if (socket_fds[i].fd == -1)
				break ;
			i ++;
		}
		std::swap(socket_fds[i], buffer_new_fd[buffer_index]);
	}
	buffer_counter = 0;
	poll_tracker = fd_counter;
}

void	Server::setupServer( void ) {
	setupSocketfds();
	typedef std::vector<ServerBlock>::iterator ServerIter;
	typedef std::vector<Socket>::iterator SocketIter;
	size_t	index = 0;
	for (ServerIter it = server_info.begin(); it != server_info.end(); it ++) {
		std::cout << "huh"<< it->listen.size() << "\n";
		for (SocketIter addr_it = it->listen.begin(); addr_it != it->listen.end(); addr_it ++) {
			socket_fds[index].fd = setListeningSocket(addr_it->refAddress(), socket_type, socket_protocol);
			std::cout << "check server fd: " << socket_fds[index].fd;
			setNonBlockFd(socket_fds[index].fd);
			server_mapping.push_back(it);
			index ++;
		}
	}
}

void	Server::getNewConnection( int fd, server_block_iter &it ) {
	if (buffer_counter == buffer_new_fd.size()) {
		return ; // too many fds in buffer
	}
	if (fd_counter == server_limit) {
		return ; // too many fds in server
	}
	Client	buffer(it);
	int fd_client = accept(fd, (sockaddr_t *)&buffer.changeAddress(),
			&buffer.getSocklen());
	if (fd_client < 0) {
		return ; // cant get
	}
	setNonBlockFd(fd_client); // need try catch?
	buffer.setFd(fd);
	buffer_new_fd[buffer_counter].fd = fd;
	client_info.push_back(buffer);
	client_mapping[fd] = -- client_info.end();
	buffer_counter ++;
	fd_counter ++;
}

void	Server::closeConnection( size_t index ) {
	close(socket_fds[index].fd);
	std::map<int, client_ptr>::iterator it = client_mapping.find(socket_fds[index].fd);
	socket_fds[index].fd = -1;
	socket_fds[index].events = POLLIN;
	if (it == client_mapping.end()) {
		return ;
	}
	client_info.erase(it->second);
	client_mapping.erase(it);
}

void	Server::loopServer( void ) {
	if (poll(getSocketfds(), poll_tracker, timeout) < 0)
		return ; // throw error?
	for (size_t index = 0; index != server_limit; index ++) {
		pollfd_t			poll_fd = socket_fds[index];
		server_block_iter&	it = server_mapping[index];
		if (poll_fd.fd < 0) {
			continue ;
		}
		if (poll_fd.revents == 0) {
			continue ;
		}
		if (poll_fd.revents & POLLIN) {
			if (index < server_no) {
				getNewConnection(poll_fd.fd, it);
			}
			else {
				Client	&ptr = *(client_mapping[poll_fd.fd]);
				receiveRequest(ptr);
				if (ptr) {
					poll_fd.events = POLLOUT;
				}

			}
		}
		if (poll_fd.revents & POLLOUT) {
			Client	&ptr = *(client_mapping[poll_fd.fd]);
			sentReponse(ptr);
			if (ptr) {
				closeConnection(index);
			}	
		}
	}
	resetFds();
}

void	Server::startServerLoop( int *signal ) {
	if (!socket_fds.size()) {
		setupServer();
	}
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

nfds_t	Server::getServerNo( void ) const {
	return (this->server_no);
}

nfds_t	Server::getServerLimit( void ) const {
	return (this->server_limit);
}

nfds_t	Server::getFdCounter( void ) const {
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
