/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:02:07 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/25 18:21:14 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

const char *Server::server_name = NULL;

Server::Server( void ) { }

Server::Server( const Server &src ) {
	*this = src;
}

Server&	Server::operator=( const Server &src )
{
	if (this == &src)
		return (*this);
	server_no = src.server_no;
	server_info = src.server_info;
	return (*this);
}

Server::~Server( void ) {
	for (size_t i = 0; i < server_no; i ++) {
		close(socket_fds[i].fd);
	}
}

bool	Server::checkBufferfds( void ) const {
	return ((buffer_counter > server_limit) ? false : true);
}

// may need to revise if use std::allocator without resever
void	Server::addBufferfds( int fd, int events ) {
	pollfd_t	new_fd = {0, 0, 0};
	new_fd.fd = fd;
	new_fd.events = events;
	buffer_new_fd.push_back(new_fd);
	buffer_counter ++;
	fd_counter ++;
}

void	Server::addBufferfds( int fd ) {
	addBufferfds(fd, POLLIN | POLLOUT);
}

void	Server::addClientContentFd( Client &client ) {
	if (!checkBufferfds()) {
		return ;
	}
	int	fd = client.getContentFd();
	if (fd < 0) {
		return ;
	}
	addBufferfds(fd);
	client.serverReceived();
}

void	Server::markAsDelete( pollfd_t &pollfd ) {
	client_mapping.erase(pollfd.fd);
	pollfd.fd = -1;
}

void	Server::markAsDelete( pollfd_t &pollfd, Client &client ) {
	markAsDelete(pollfd);
	client.markforDeletion();
}

void	Server::error2Client( int fd, client_ptr client ) {
	if (client->clientSocketFd() == fd) {
		client->markforDeletion();
		return ;
	}
	client->errorOverwriteResponse(500);
}

int	Server::setupSocketsCheckError( listen_ptr ptr, addrinfo_ptr addr ) {
	int error = 1;
	int	fd = -1;
	while (error) {
		fd = ptr->addListenFd(addr);
		if (fd > 0) {
			return (fd);
		}
		if (error == 4) {
			break ;
		}
		error ++;
	}
	//maybe throw error
	return (-1);
}

void	Server::setupSocketsListen( serverinfo_ptr ptr, pollfd_t &buffer ) {
	size_t	counter = 0;
	for (listen_ptr it = ptr->listenBegin(); it != ptr->listenEnd(); it ++) {
		for (addrinfo_ptr addr = it->begin(); addr != it->end(); addr ++) {
			buffer.fd = setupSocketsCheckError(it, addr);
			if (buffer.fd < 0) {
				return ; // error?
			}
			socket_fds.push_back(buffer);
			socketfd_mapping.push_back(addr);
			counter ++;
		}	
	}
	server_mapping.insert(server_mapping.end(), counter, ptr);
	server_no += counter;	
}

void	Server::setupSockets( void ) {
	pollfd_t	pollfd;
	pollfd.fd = -1;
	pollfd.events = POLLIN;
	pollfd.revents = 0;
	server_no = 0;
	for (serverinfo_ptr it = server_info.begin();
		 it != server_info.end(); it ++) {
		setupSocketsListen(it, pollfd);
	}
	fd_counter = server_no;
	poll_tracker = fd_counter;	
	server_limit = socket_fds.max_size();
	// if (server_no > server_limit)
		// return ;
	uint64_t	num = server_no * 3;
	socket_fds.reserve((num > server_limit) ? server_limit : num);
	num = server_no * 2;
	buffer_new_fd.reserve((num > server_limit) ? server_limit : num);
}

void	Server::resetFds( void ) { // erase mark as deleted client somehow
	typedef std::vector<pollfd_t>::const_iterator iter;
	for (iter i = socket_fds.begin(); i != socket_fds.end(); i ++) {
		if (i->fd < 0) {
			socket_fds.erase(i);
		}
	}
	for (nfds_t i = 0; i < buffer_counter; i ++) {
		socket_fds.push_back(buffer_new_fd[i]);
	}
	buffer_new_fd.clear();
	buffer_counter = 0;
	poll_tracker = fd_counter;
}


void	Server::handleClientRecv( pollfd_t &poll_fd, Client &client ) {
	if (poll_fd.fd == client.clientSocketFd()) {
		if (!client.clientRecvHttp()) {
			markAsDelete(poll_fd, client);
			return ;
		}
		return ;
	}
	if (!client.clientRecvContent()) {
		markAsDelete(poll_fd);
		return ;
	}
}

void	Server::handleClientSent( pollfd_t &poll_fd, Client &client ) {
	if (!client.checkResponseStatus()) {
		return ;
	}
	if (poll_fd.fd == client.clientSocketFd()) {
		if (!client.clientSendResponse()) {
			markAsDelete(poll_fd, client);
		}
	}
}

void	Server::handleClient( size_t index ) {
	pollfd_t	&poll_fd = socket_fds[index];
	if (poll_fd.fd < 0) {
		return ;
	}
	if (!poll_fd.revents) {
		return ;
	}
	client_ptr ptr = client_mapping[poll_fd.fd];
	if (ptr->toBeDeleted()) {
		return ;
	}
	if (poll_fd.revents & POLLIN) {
		handleClientRecv(poll_fd, *ptr);
	}
	if (poll_fd.revents & POLLOUT) {
		handleClientSent(poll_fd, *ptr);
	}
	if (poll_fd.revents & (POLLHUP | POLLERR)) {
		error2Client(poll_fd.fd, ptr);
	}
	if (ptr->giveContentFdtoServer()) {
		addClientContentFd(*ptr);
	}
	// check timer , abort(etc invalid header) 
}

void	Server::handleServer( size_t index ) {
	if (!(socket_fds[index].revents & POLLIN)) {
		return ;
	}
	if (!checkBufferfds()) {
		return ;
	}
	Client	buffer(server_mapping[index]);
	int	fd = buffer.clientSocketFd(socket_fds[index].fd);
	if (fd < 0) {
		return ;
	}
	addBufferfds(fd);
	client_info.push_back(buffer);
}

void	Server::loopServer( void ) {
	if (poll(getSocketfds(), poll_tracker, timeout) < 0) {
		return ; // throw error?
	}
	for (size_t index = server_no; index < fd_counter; index ++) {
		handleClient(index);
	}
	for (size_t index = 0; index != server_no; index ++) {
		handleServer(index);
	}
	resetFds();
}

void	Server::startServerLoop( void ) {
	if (!socket_fds.size()) {
		setupSockets();
	}
	if (!server_no) {
		// std::cout << "no servers lulz\n";
		return ;
	}
	while (1) {
		loopServer();
	}
}

void	Server::addServerInfo( ServerInfo &ref ) {
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

int	Server::getTimeout( void ) const {
	return (this->timeout);
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

// std::ostream&	operator<<( std::ostream &o, const Server& ref ) {
// 	o << "Server Status\n";
// 	o << "Server info\n";
// 	// o << "Display Sockets: " << ref.displaySocketFds(o) << '\n';
// 	// o << "Servers info\n" << ref.displayServerInfo(o) << '\n';
// 	return (o);
// }
