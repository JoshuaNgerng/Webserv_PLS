/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:02:07 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/31 15:56:50 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

const char		*Server::server_name = NULL;
volatile bool	Server::running = true;

Server::Server( void ) :
server_no(0),
server_limit(0),
fd_counter(0),
buffer_counter(0),
poll_tracker(0),
socket_fds(),
buffer_new_fd(),
server_mapping(),
socketfd_mapping(),
client_mapping(),
server_info(),
client_info()
{ }

Server::Server( const Server &src ) {
	*this = src;
}

Server&	Server::operator=( const Server &src )
{
	if (this == &src)
		return (*this);
	server_no = src.server_no;
	server_limit = src.server_limit;
	fd_counter = src.fd_counter;
	buffer_counter = src.buffer_counter;
	poll_tracker = src.poll_tracker;
	socket_fds = src.socket_fds;
	buffer_new_fd = src.buffer_new_fd;
	server_mapping = src.server_mapping;
	socketfd_mapping = src.socketfd_mapping;
	client_mapping = src.client_mapping;
	server_info = src.server_info;
	client_info = src.client_info;
	server_no = src.server_no;
	server_info = src.server_info;
	return (*this);
}

Server::~Server( void ) {
	// std::cout << server_no << '\n';
	for (size_t i = 0; i < server_no && i < socket_fds.size(); i ++) {
		close(socket_fds[i].fd);
	}
}

void	Server::signalHandler( int signal ) {
	std::cout << "Signal received: " << signal << '\n';
	if (signal != SIGINT) {
		return ;
	}
	running = false; // Set flag to indicate cleanup
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

void	Server::addClientContentFd( client_ptr client ) {
	if (!checkBufferfds()) {
		return ;
	}
	int	fd = client->getContentFd();
	if (fd < 0) {
		return ;
	}
	std::cout << "client content name: " << client->getContentName() << '\n';
	addBufferfds(fd);
	std::cout << "client added content fd: " << fd << '\n';
	client->serverReceived();
}

void	Server::markAsDelete( pollfd_t &pollfd ) {
	std::cout << "pollfd mark as delete\n";
	pollfd.fd = -1;
	fd_counter --;
}

void	Server::markAsDelete( pollfd_t &pollfd, Client &client ) {
	std::cout << "client mark as delete\n";
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

void	Server::resetFds( void ) {
	typedef std::vector<pollfd_t>::iterator	iter;
	iter it = socket_fds.begin();
	for (size_t i = 0; i < server_no; i ++) {
		if (it == socket_fds.end()) {
			break ;
		}
		it ++;
	}
	while (it != socket_fds.end()) {
		if (it->fd < 0) {
			it = socket_fds.erase(it);
		} else {
			it ++;
		}
	}
	std::cout << "new fd added\n";
	for (nfds_t i = 0; i < buffer_counter; i ++) {
		socket_fds.push_back(buffer_new_fd[i]);
		std::cout << socket_fds.back().fd << ", ";
	}
	std::cout << '\n';
	std::cout << "socket fd status at reset\n";
	for (client_ptr ptr = client_info.begin(); ptr != client_info.end();) {
		if (ptr->toBeDeleted()) {
			ptr = client_info.erase(ptr);
			std::cout << "client sucessfully deleted\n";
		} else {
			ptr ++;
		}
	}
	buffer_new_fd.clear();
	buffer_counter = 0;
	poll_tracker = fd_counter;
	for (size_t i = 0; i < poll_tracker; i ++) {
		std::cout << socket_fds[i].fd << ", ";
	}
	std::cout << "\n";
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
	if (!client.checkResponseReady()) {
		std::cout << "client check response not ready\n";
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
	std::cout << "test client\n";
	client_ptr ptr = client_mapping[poll_fd.fd];
	if (ptr->toBeDeleted()) {
		return ;
	}
	if (poll_fd.revents & POLLIN) {
		std::cout << "test POLLIN\n";
		handleClientRecv(poll_fd, *ptr);
	}
	if (poll_fd.revents & POLLOUT) {
		std::cout << "test POLLOUT\n";
		handleClientSent(poll_fd, *ptr);
	}
	if (poll_fd.revents & (POLLHUP | POLLERR)) {
		std::cout << "test POLLERR\n";
		error2Client(poll_fd.fd, ptr);
	}
	if (ptr->giveContentFdtoServer()) {
		addClientContentFd(ptr);
	}
	std::cout << "try client end\n";
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
	client_mapping[fd] = -- client_info.end();
	std::cout << "client socket successfully added\n";
}

void	Server::loopServer( void ) {
	std::cout << "poll tracker: " << poll_tracker << " fd_counter " << fd_counter << '\n';
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
		return ;
	}
	std::cout << "running" << running << "\n";
	while (running) {
		loopServer();
	}
}

void	Server::clearListenAddr( void ) {
	typedef std::vector<ServerInfo>::iterator	iter;
	for (iter ptr = server_info.begin(); ptr != server_info.end(); ptr ++) {
		ptr->clearListenAddr();
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
