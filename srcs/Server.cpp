/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:55:53 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/20 17:40:40 by jngerng          ###   ########.fr       */
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
	server_mapping.insert(server_mapping.end(), server_no, 0);
}

void	Server::setupServer( void ) {
	typedef std::vector<ServerBlock>::iterator ServerIter;
	typedef std::vector<Socket>::iterator SocketIter;
	size_t	server_index = 0;
	size_t	serverblock_index = 0;
	for (ServerIter it = server_info.begin(); it != server_info.end(); it ++) {
		std::cout << "huh"<< it->listen.size() << "\n";
		for (SocketIter addr_it = it->listen.begin(); addr_it != it->listen.end(); addr_it ++) {
			const sockaddr_in_t	&addr = addr_it->refAddress();
			socket_fds[server_index].fd = socket(addr.sin_family, socket_type, socket_protocol);
			std::cout << "check server fd: " << socket_fds[server_index].fd;
			if (socket_fds[server_index].fd < 0) {
				std::cout << "socket failed\n";
				return ;
			}
			if (bind(socket_fds[server_index].fd, (sockaddr *)&addr, socklen) < 0) {
				std::cout << "bind failed\n";
				return ;
			}
			if (listen(socket_fds[server_index].fd, backlog_limit) < 0) {
				std::cout << "listen failed\n";
				return ;
			}
			server_mapping[server_index] = serverblock_index;
			server_index ++;
		}
		serverblock_index ++;
	}
}

void	Server::getNewConnection( size_t index ) {
	if (server_index == server_limit) {
		std::cout << "max connection capacity met\n";
		return ;
	}
	uint32_t	slot = findAvaliableSlot();
	if (slot == server_limit)
		return ;
	Client	new_client(index);
	int		fd = -1;
	int		flags = 0;
	std::cout << "test on " << server_info[server_mapping[index]].listen[0];
	fd = accept(socket_fds[index].fd,
			(sockaddr *)&new_client.changeAddress() , &new_client.getSocklen());
	if (fd < 0) {
		std::cout << "accept fail\n";
		return ; // cant accept socket error?
	}
	if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
		close(fd);
		return ; // get flag error?
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
		close(fd);
		return ; // set flag error?
	}
	new_client.getFd() = fd;
	socket_fds[slot].fd = fd;
	client_info.push_back(new_client);
	client_mapping[fd] = -- client_info.end();
	server_index ++;
}

void	Server::closeConnection( size_t index ) {
	if (socket_fds[index].fd < 0)
		return ;
	close(socket_fds[index].fd);
	client_ptr pos = client_mapping[socket_fds[index].fd];
	client_mapping.erase(socket_fds[index].fd);
	client_info.erase(pos);
	socket_fds[index].fd = -1;
	socket_fds[index].events = POLLIN;
}

bool	Server::receiveRequest( Client &client )
{
	char	buffer[buffer_limit];
	ssize_t	bytes;

	bytes = recv(client.getFd(), buffer, buffer_limit, recv_flag);
	if (!bytes)
		return (false);
	if (bytes < 0)
		return (false);
	std::string	str(buffer);
	if (str.find("\r\n\r\n") != std::string::npos) {
		client.finishRecv();
	}
	client.addToReq(str);
	return (true);
}

void	Server::fetchData( Client &client ) {
	ServerBlock	&ref = server_info[client.getServerRef()];
	client.addToRes(ref.testHTML());
}

bool	Server::sentReponse( Client &client )
{
	const std::string msg = client.getResponse().substr(client.getBytesSent());
	size_t	len = client.getResponse().length() - client.getBytesSent();
	ssize_t	bytes = send(client.getFd(), msg.c_str(), len, send_flag);
	if (bytes == 0)
		return (false); // error close connection
	if (bytes < 0)
		return (false); // error cant write
	client.addBytesSent(bytes);
	if (client.getBytesSent() == client.getResponse().length())
		client.finishSend();
	return (true);
}

// assume client start with pollin 
// then move to pollout
void	Server::loopServer( void ) {
	if (poll(getSocketfds(), server_limit, timeout) < 0)
		return ; // throw error?
	std::cout << "check: " << socket_fds[0].fd << '\n';
	for (size_t index = server_limit - 1; index != 0; index --) {
		pollfd_t	ref_fd = socket_fds[index];
		std::cout << index <<") test: " << ref_fd.fd << '\n';
		if (ref_fd.fd < 0) {
			continue ;
		}
		if (index < server_no) {
			std::cout << "process server\n";
			if (ref_fd.revents & POLLIN)
				getNewConnection(ref_fd.fd);
			continue ;
		}
		Client	&client = *(client_mapping[ref_fd.fd]);
		if (ref_fd.revents & POLLIN) {
			if (!receiveRequest(client))
				closeConnection(index);
			if (client.checkReq()) {
				ref_fd.events = POLLOUT;
				fetchData(client);
			}
		}
		if (ref_fd.revents & POLLOUT) {
			if (!sentReponse(client))
				closeConnection(index);
			if (client.checkRes())
				closeConnection(index);
		}
		if (ref_fd.revents & (POLLHUP | POLLERR)) {
			closeConnection(index);
		}
	}
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
	ServerBlock	buffer;
	server_info.push_back(buffer);
	ft_move(server_info.back(), ref);
}

pollfd_t*	Server::getSocketfds( void ) { return (&(*(socket_fds.begin()))); }


//getters
const socklen_t	Server::getSocklen( void ) const {
	return (this->socklen);
}

const int	Server::getSocketType( void ) const {
	return (this->socket_type);
}

const int	Server::getSocketProtocol( void ) const {
	return (this->socket_protocol);
}

const int	Server::getBacklogLimit( void ) const {
	return (this->backlog_limit);
}

const int	Server::getRecvFlag( void ) const {
	return (this->recv_flag);
}

const int	Server::getSendFlag( void ) const {
	return (this->send_flag);
}

const int	Server::getTimeout( void ) const {
	return (this->timeout);
}

const int	Server::getBufferLimit( void ) const {
	return (this->buffer_limit);
}

uint32_t	Server::getServerNo( void ) const {
	return (this->server_no);
}

nfds_t	Server::getServerLimit( void ) const {
	return (this->server_limit);
}

uint32_t	Server::getServerIndex( void ) const {
	return (this->server_index);
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

std::ostream&	Server::displaySocketFds( std::ostream &o ) const {
	return (displayContainer(o, socket_fds));
}
//end of display

std::ostream&	operator<<( std::ostream &o, const Server& ref ) {
	o << "Servers Status\n";
	o << "Const info: " << "backlog limit: " << ref.getBacklogLimit() <<
		", recv flag: " << ref.getRecvFlag() << ", send flag: " << ref.getSendFlag() <<
		", poll timeout: " << ref.getTimeout() << ", recv buffer size: " << ref.getBufferLimit() << '\n';
	o << "Display Sockets: " << ref.displaySocketFds(o);
	o << "Server status\n";
	o << ref.displayServerInfo(o);
	return (o);
}
