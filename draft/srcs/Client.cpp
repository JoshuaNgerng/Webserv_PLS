/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/01 12:20:05 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) { }

// Client::Client( std::vector<ServerInfo>::iterator &it, int fd ) :

// Client::Client( const Client &src ) : 

Client::~Client( void ) { }

Client&	Client::operator=( const Client &src ) {
	if (this == &src)
		return (*this);
	// server_ref = src.server_ref; socket_fd = src.socket_fd;
	// socket_addr = src.socket_addr; len = src.len; request_header = src.request_header;
	// request_body = src.request_body; response = src.response; bytes_sent = src.bytes_sent;
	// got_header = src.got_header; got_body = src.got_body; finish_response = src.finish_response;
	return (*this);
}

int	Client::clientSocketFd( int listen_fd ) {
	socket_fd = accept(listen_fd, (sockaddr_t *)(&client_addr), &len);
	if (socket_fd < 0) {
		return (-1);
	}
	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0) {
		close(socket_fd);
		return (-1);
	}
	return (socket_fd);
}

bool	Client::clientRecv( void ) {
	char	buffer[recv_buffer_size + 1];
	ssize_t	r = recv(socket_fd, buffer, recv_buffer_size, recv_flag);
	if (r == 0) {
		return (false);
	}
	if (r == -1) {
		return (false);
	}
	buffer[r] = '\0';
	std::string	str(buffer);
	if (!requests.size()) { 
		HttpRequest new_req;
		requests.push(new_req);
	}
	size_t pos = requests.back().addRequest(str);
	if (pos == std::string::npos) {
		return (false);
	}
	while (pos) {
		HttpRequest new_req;
		requests.push(new_req);
		str.erase(0, str.length() - pos);
		pos = requests.back().addRequest(str);
		if (pos == std::string::npos) {
			return (false);
		}
	}
	return (true);
}

int	Client::findResource( void ) {
	const HttpRequest &req = requests.front();
	if (!req.getValidHeader()) {
		return (-1);
	}
	if (resource_name.length()) {
		return (resource_fd);
	}
	server_ref->matchUri(resource_name, req.getUri());
	return (resource_fd);
}

bool	Client::checkReponse( void ) {
	
}

std::vector<ServerInfo>::iterator	Client::getServerRef( void ) const {
	return(server_ref);
}

int	Client::clientSocketFd( void ) const { return (socket_fd); }

int	Client::getResourceFd( void ) const { return(resource_fd); }

std::ostream&	operator<<( std::ostream &o, const Client &ref ) {
	o << "Client socket fd: " << ref.clientSocketFd() << '\n';
	o << "Client reponse fd: " << ref.getResourceFd();// <<
		// ", reponse resource status: " << ((ref.isDataReady()) ? "ready" : "not ready") << '\n';
	o << "Request status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	o << "Request from Client\n" << ref.getRequest() << '\n';
	o << "Reponse status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	o << "Reponse to Client\n" << ref.getResponse() << '\n';
	return (o);
}

// size_t	Client::getBytesSent( void ) const { return(bytes_sent); }
