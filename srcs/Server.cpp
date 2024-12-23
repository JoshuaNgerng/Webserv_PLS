/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:02:07 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/15 03:11:12 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

const char*		Server::setup_err_msg[] = { "socket", "bind", "listen" };

const char*		Server::server_name = "";

char *const*	Server::env = NULL;

volatile bool	Server::running = true;

Server::Server( void ) :
server_no(0),
server_limit(0),
fd_counter(0),
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
	for (size_t i = 0; i < server_no && i < socket_fds.size(); i ++) {
		close(socket_fds[i].fd);
	}
}

void	Server::signalHandler( int signal ) {
	std::cout << "Signal received " << signal 
	<< ", ending server gracefully\n";
	if (signal != SIGINT) {
		return ;
	}
	running = false;
}

bool	Server::checkBufferfds( size_t no ) const {
	return ((buffer_new_fd.size() + no > server_limit) ? false : true);
}

void	Server::addBufferfds( int fd, int events ) {
	pollfd_t	new_fd;
	new_fd.fd = fd;
	new_fd.events = events;
	new_fd.revents = 0;
	buffer_new_fd.push_back(new_fd);
	fd_counter ++;
}

void	Server::addBufferfds( int fd ) {
	addBufferfds(fd, POLLIN | POLLOUT);
}

void	Server::addSingleFd( int fd, int event, client_ptr client ) {
	addBufferfds(fd, event);
	client_mapping.insert(std::pair<int, client_ptr>(fd, client));
}

void	Server::addClientContentFd( client_ptr client ) {
	const File	*ptr = client->getContent();
	if (!ptr) {
		return ;
	}
	if (!checkBufferfds(ptr->getFdCounter())) {
		return ;
	}
	addSingleFd(ptr->getInputFd(), POLLIN, client);
	if (ptr->getFdCounter() > 1) {
		addSingleFd(ptr->getOutputFd(), POLLOUT, client);
	}
	client->serverReceived();
}

void	Server::markAsDelete( int fd ) {
	fds_to_be_deleted.push_back(fd);
	fd_counter --;
}

void	Server::markAsDelete( const File *ptr ) {
	if (!ptr) {
		return ;
	}
	fds_to_be_deleted.push_back(ptr->getInputFd()); // always have input fd
	if (ptr->getFdCounter() > 1) {
		fds_to_be_deleted.push_back(ptr->getOutputFd()); // may or may not have output fd
	}
	fd_counter -= ptr->getFdCounter();
}

void	Server::markAsDelete( Client &client ) {
	client.markforDeletion();
	fds_to_be_deleted.push_back(client.clientSocketFd()); // input socket
	fds_to_be_deleted.push_back(client.clientSocketFd()); // output socket
	fd_counter -= 2;
	if (!client.getContent()) {
		return ;
	}
	markAsDelete(client.getContent());
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
		std::cerr << server_name << ": [emerg] " << setup_err_msg[fd + 2] <<
			"() to " << *ptr << " failed\n";
	}
	return (fd);
}

void	Server::setupSocketsListen( serverinfo_ptr ptr, pollfd_t &buffer ) {
	size_t	counter = 0;
	for (listen_ptr it = ptr->listenBegin(); it != ptr->listenEnd(); it ++) {
		for (addrinfo_ptr addr = it->begin(); addr != it->end(); addr ++) {
			buffer.fd = setupSocketsCheckError(it, addr);
			if (buffer.fd < 0) {
				std::cerr << server_name << ": [emerg] still could not " 
					<< setup_err_msg[buffer.fd + 2] << "()\n";
				throw SetupError();
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
	server_limit = socket_fds.max_size();
	// if (server_no > server_limit)
		// return ;
	uint64_t	num = server_no * 3;
	socket_fds.reserve((num > server_limit) ? server_limit : num);
	num = server_no * 2;
	buffer_new_fd.reserve((num > server_limit) ? server_limit : num);
}

bool	Server::checkFdDeletion( int fd ) {
	typedef std::list<int>::iterator iter;
	for (iter it = fds_to_be_deleted.begin(); it != fds_to_be_deleted.end(); it ++) {
		if (*it == fd) {
			fds_to_be_deleted.erase(it);
			return (true);
		}
	}
	return (false);
}

void	Server::resetFds( void ) {
	typedef std::vector<pollfd_t>::iterator	iter;
	iter	it = socket_fds.begin();
	for (size_t i = 0; i < server_no; i ++) {
		if (it == socket_fds.end()) {
			break ;
		}
		it ++;
	}
	while (it != socket_fds.end()) {
		if (checkFdDeletion(it->fd)) {
			client_mapping.erase(it->fd);
			it = socket_fds.erase(it);
		} else {
			it ++;
		}
	}
	for (nfds_t i = 0; i < buffer_new_fd.size(); i ++) {
		socket_fds.push_back(buffer_new_fd[i]);
	}
	for (client_ptr ptr = client_info.begin(); ptr != client_info.end();) {
		if (ptr->toBeDeleted()) {
			client_mapping.erase(ptr->clientSocketFd());
			if (ptr->getInputFd() > 0) {
				client_mapping.erase(ptr->getInputFd());
			}
			if (ptr->getOutputFd() > 0) {
				client_mapping.erase(ptr->getOutputFd());
			}
			ptr = client_info.erase(ptr);
		} else {
			ptr ++;
		}
	}
	buffer_new_fd.clear();
	fds_to_be_deleted.clear();
	fd_counter = socket_fds.size();
}

void	Server::cleanUpClient( int fd, client_ptr client ) {
	if (client->clientSocketFd() == fd) {
		client->markforDeletion();
		return ;
	}
	if (!client->checkContentStatus()) {
		client->errorOverwriteResponse(500);
	}
	markAsDelete(fd);
}

void	Server::error2Client( int fd, client_ptr client ) {
	if (client->clientSocketFd() == fd) {
		client->markforDeletion();
		return ;
	}
	client->errorOverwriteResponse(500);
	markAsDelete(fd);
}

void	Server::handleClientRecv( pollfd_t &poll_fd, Client &client ) {
	if (poll_fd.fd == client.clientSocketFd()) {
		if (!client.clientRecvHttp()) {
			markAsDelete(client);
			return ;
		}
		return ;
	}
	if (!client.clientRecvContent()) {
		if (!client.checkResponseReady()) {
			markAsDelete(client.getContent());
		} else {
			markAsDelete(poll_fd.fd);
		}
	}
}

void	Server::handleClientSent( pollfd_t &poll_fd, Client &client ) {
	if (poll_fd.fd != client.clientSocketFd()) {
		if (!client.clientSendContent()) {
			markAsDelete(poll_fd.fd);
		}
		return ;
	}
	if (!client.checkResponseReady()) {
		return ;
	}
	if (!client.clientSendResponse()) {
		markAsDelete(client);
	}
}

void	Server::handleClient( size_t index ) {
	typedef std::map<int, client_ptr>::iterator	iter;
	pollfd_t	&poll_fd = socket_fds[index];
	if (poll_fd.fd < 0) {
		return ;
	}
	iter check = client_mapping.find(poll_fd.fd);
	if (check == client_mapping.end()) {
		return ;
	}
	client_ptr ptr = check->second;
	if (ptr->toBeDeleted()) {
		return ;
	}
	if (!poll_fd.revents) {
		ptr->setNewCurrentTime();
	} else if (poll_fd.revents & POLLIN) {
		handleClientRecv(poll_fd, *ptr);
	} else if (poll_fd.revents & POLLOUT) {
		handleClientSent(poll_fd, *ptr);
	} else if (poll_fd.revents & POLLHUP) {
		cleanUpClient(poll_fd.fd, ptr);
	} else if (poll_fd.revents & POLLERR) {
		error2Client(poll_fd.fd, ptr);
	}
	if (!ptr->giveContentFdtoServer()) {
		addClientContentFd(ptr);
	}
	int status = ptr->checkTimer(poll_fd.fd);
	if (status > 200) {
		if (poll_fd.fd != ptr->clientSocketFd()) {
			markAsDelete(ptr->getContent());
		}
		ptr->errorOverwriteResponse(status);
	}
}

void	Server::handleServer( size_t index ) {
	if (!(socket_fds[index].revents & POLLIN)) {
		return ;
	}
	if (!checkBufferfds(2)) {
		return ;
	}
	Client	buffer(server_mapping[index]);
	int	fd = buffer.clientSocketFd(socket_fds[index].fd);
	if (fd < 0) {
		return ;
	}
	addBufferfds(fd, POLLIN);
	addBufferfds(fd, POLLOUT);
	client_info.push_back(buffer);
	std::list<Client>::iterator iter = -- client_info.end();
	client_mapping[fd] = iter;
	iter->setStartConnectionTime();
	iter->setNewCurrentTime();
	buffer.ignoreClosingFd();
}

void	Server::loopServer( void ) {
	if (poll(getSocketfds(), socket_fds.size(), timeout) < 0) {
		return ; // throw error?
	}
	for (size_t index = server_no; index < socket_fds.size(); index ++) {
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
	std::cout << "running\n";
	displayServerInfo(std::cout);
	while (running) {
		loopServer();
	}
}

void	Server::normalizeDefaultSetting( void ) {
	typedef std::vector<ServerInfo>::iterator	server_iter;
	for (server_iter it = server_info.begin(); it != server_info.end(); it ++) {
		it->defaultSetting();
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
	o << "fd: " << ref.fd << ", events:";
	if (ref.events & POLLIN) { o << "POLLIN, "; } 
	if (ref.events & POLLOUT) { o << "POLLOUT, "; }
	o << "revents: ";
	if (ref.revents & POLLIN) { o << "POLLIN, "; } 
	if (ref.revents & POLLOUT) { o << "POLLOUT, "; }
	return (o);
}

const char* Server::SetupError::what() const throw() {
	return ("Cannot setup server sockets");
}

// std::ostream&	operator<<( std::ostream &o, const Server& ref ) {
// 	o << "Server Status\n";
// 	o << "Server info\n";
// 	// o << "Display Sockets: " << ref.displaySocketFds(o) << '\n';
// 	// o << "Servers info\n" << ref.displayServerInfo(o) << '\n';
// 	return (o);
// }
