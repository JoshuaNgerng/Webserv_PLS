/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:02:07 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/15 14:41:12 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void ) { }

Server::Server( const Server &src ) { *this = src; }
// 	server_no(src.server_no), client_limit(src.client_limit),
// 	server_fds(src.server_fds), client_fds(src.client_fds),
// 	server_info(src.server_info) { }

Server::~Server( void ) { }

Server&	Server::operator=( const Server &src )
{
	if (this == &src)
		return (*this);
	server_no = src.server_no;
	socket_fds = src.socket_fds;
	buffer_new_fd = src.buffer_new_fd;
	server_mapping = src.server_mapping;
	client_mapping = src.client_mapping;
	server_info = src.server_info;
	client_info = src.client_info;
	return (*this);
}

int	Server::setListeningSocket( const sockaddr_in_t &addr, int socket_type, int socket_protocol ) {
	int	fd = socket(addr.sin_family, socket_type, socket_protocol);
	if (fd < 0) {
		goto error; // cant set socket
	}
	if (bind(fd, (sockaddr *)&addr, socklen) < 0) {
		// std::cout << "bind failed\n";
		goto clear_fd; // bind failed
	}
	if (listen(fd, backlog_limit) < 0) {
		// std::cout << "listen failed\n";
		goto clear_fd; // listen failed
	}
	return (fd);
	clear_fd:
		close(fd);
	error:
		return (-1);
}

bool	Server::checkBufferfds( void ) const {
	if (buffer_counter == buffer_new_fd.size()) {
		return (false); // too many fds in buffer
	}
	if (fd_counter == server_limit) {
		return (false); // too many fds in server
	}
	return (true);
}

void	Server::addBufferfds( int fd ) {
	if (fd < 0)
		return ;
	buffer_new_fd[buffer_counter].fd = fd;
	buffer_counter ++;
	fd_counter ++;
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
		if (socket_fds[i].fd > -1) {
			continue ;
		}
		if (buffer_index < buffer_counter) {
			std::swap(buffer_new_fd[buffer_index], socket_fds[i]);
			buffer_index ++;
		}
		else {
			for (nfds_t empty = i; empty < poll_tracker; empty ++) {
				if (socket_fds[empty].fd > -1) {
					std::swap(socket_fds[empty], socket_fds[i]);
					break ;
				}
			}
		}		
	}
	for (; buffer_index < buffer_counter; buffer_index ++) {
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
		// std::cout << "huh"<< it->listen.size() << "\n";
		for (SocketIter addr_it = it->listen.begin();
			addr_it != it->listen.end(); addr_it ++) {
			socket_fds[index].fd = setListeningSocket(addr_it->refAddress(),
						socket_type, socket_protocol);
			// std::cout << "check server fd: " << socket_fds[index].fd;
			if (fcntl(socket_fds[index].fd, F_SETFL, fcntl_flag) < 0) {
				close(socket_fds[index].fd);
				return ; // error cant set fl for fd
			}
			server_mapping.push_back(it);
			index ++;
		}
	}
}

void	Server::getNewConnection( int fd, serverblock_ptr &it ) {
	if (!checkBufferfds())
		return ;
	Client	buffer(it);
	int fd_client = accept(fd, (sockaddr_t *)&buffer.changeAddress(),
			&buffer.getSocklen());
	if (fd_client < 0) {
		return ; // cant get
	}
	if (fcntl(fd_client, F_SETFL, fcntl_flag) < 0) {
		close(fd_client);
		return ; // error cant set fl for fd
	}
	buffer.setSocketFd(fd);
	client_info.push_back(buffer);
	client_mapping[fd] = -- client_info.end();
	addBufferfds(fd);
}

void	Server::resetPollFd( pollfd_t &pollfd )
{
	pollfd.fd = -1;
	pollfd.events = POLLIN;
	pollfd.revents = 0;
}

void	Server::clearClient( size_t index ) {
	int	fd = socket_fds[index].fd;
	resetPollFd(socket_fds[index]);
	fd_counter --;
	std::map<int, client_ptr>::iterator it1, it2;
	it1 = client_mapping.find(socket_fds[index].fd);
	if (it1 == client_mapping.end()) {
		return ;
	}
	client_ptr	ptr = it1->second;
	int			fd2 = -1;
	close(fd);
	it2 = client_mapping.end();
	fd2 = (fd == ptr->getSocketFd()) ? ptr->getReponseFd() : ptr->getSocketFd();
	if (fd2 > 0)
	{
		it2 = client_mapping.find(fd2);
		close(fd2);
	}
	client_mapping.erase(it1);
	if (it2 != client_mapping.end())
		client_mapping.erase(it2);
	client_info.erase(ptr);
}

void	Server::fetchClientData( client_ptr &ptr ) {
	if (!checkBufferfds())
		return ;
	int	fd = ptr->fetchDataFd();
	if (fd < 0) {
		return ;
	}
	if (fcntl(fd, F_SETFL, fcntl_flag) < 0) {
		goto error;
		return ; // error cant set fl for fd
	}
	// buffer.setSocketFd(fd);
	// client_info.push_back(buffer);
	client_mapping[fd] = ptr;
	addBufferfds(fd);	
	return ;
	error:
		close(fd);
		ptr->resetDataFd();
}

void	Server::handleServer( size_t index ) {
	pollfd_t	&poll_fd = socket_fds[index];
	if (!(poll_fd.revents & POLLIN)) {
		return ;
	}
	getNewConnection(poll_fd.fd, server_mapping[index]);
}

void	Server::handleClientRecv( pollfd_t &pollfd, client_ptr &ptr, size_t index )
{
	char	buffer[buffer_limit];
	ssize_t	bytes = recv(pollfd.fd, buffer, buffer_limit, recv_flag);
	if (bytes < 0) {
		clearClient(index);
	}
	if (pollfd.fd == ptr->getSocketFd()) {
		if (!bytes) {
			clearClient(index);
		}
		ptr->addToRequestHeader(buffer, bytes);
		if (bytes > 4 && !ft_strncpy(&buffer[bytes - 4], "\r\n\r\n", 4)) {
			ptr->finishReceiveHeader();
			if (ptr->checkRequest()) {
				pollfd.events = POLLOUT;
				fetchClientData(ptr);
			}
		}
	}
	if (!bytes) {
		close(pollfd.fd);
		resetPollFd(pollfd);
		return ;
	}
	ptr->addToResponse(buffer, bytes);
}

void	Server::handleClientSent( pollfd_t &pollfd, client_ptr &ptr, size_t index ){
	if (pollfd.fd == ptr->getSocketFd()) {
		if (ptr->isDataReady() == false) {
			fetchClientData(ptr); // . try add fd
		}
		if (ptr->isDataAvaliable()) {
			clearClient(index);
			return ;
		}
		if (ptr->isTimeout()) {
			clearClient(index);
			return ;
		}
		if (ptr->isDataReady()) {
			return ;
		}
		size_t	remaining_len = ptr->getResponse().length() - ptr->getBytesSent();
		ssize_t	bytes = send(pollfd.fd,
							(ptr->getResponse().c_str() - remaining_len),
							remaining_len, 0);
		if (bytes < 0) {
			clearClient(index);
			return ;
		}
		if (!bytes) {
			clearClient(index);
			return ;
		}
		ptr->addBytesSent(bytes);
		if (ptr->checkResponse())
			clearClient(index);
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
	if (poll_fd.revents & POLLIN) {
		handleClientRecv(poll_fd, client_mapping[poll_fd.fd], index);
	}
	else if (poll_fd.revents & POLLOUT) {
		handleClientSent(poll_fd, client_mapping[poll_fd.fd], index);
	}
	else if (poll_fd.revents & (POLLHUP | POLLERR)) {
		clearClient(index);
	}
}

void	Server::loopServer( void ) {
	if (poll(getSocketfds(), poll_tracker, timeout) < 0) {
		return ; // throw error?
	}
	for (size_t index = server_no; index < fd_counter; index ++) { // handle Client first
		handleClient(index);
	}
	for (size_t index = 0; index != server_limit; index ++) {
		handleServer(index);
	}
	resetFds();
}

void	Server::startServerLoop( int *signal ) {
	if (!socket_fds.size()) {
		setupServer();
	}
	if (!server_no) {
		// std::cout << "no servers lulz\n";
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

std::ostream&	operator<<( std::ostream &o, std::ostream &display ) {
	(void)o; return (display);
}

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
	o << "Server info\n";
	o << "Display Sockets: " << ref.displaySocketFds(o) << '\n';
	o << "Servers info\n" << ref.displayServerInfo(o) << '\n';
	return (o);
}

void	Server::resetServer( void )
{
	swap_clear(socket_fds);
	swap_clear(buffer_new_fd);
	swap_clear(server_mapping);
	swap_clear(client_mapping);
	swap_clear(server_info);
	swap_clear(client_info);
}

void	Server::testAlloc( void )
{
	socket_fds.reserve(10);
	buffer_new_fd.reserve(100);
	ServerBlock	server;
	server_info.insert(server_info.end(), 10, server);
}