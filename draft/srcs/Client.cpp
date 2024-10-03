/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/02 12:00:49 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client( void ) { }

// Client::Client( std::vector<ServerInfo>::iterator &it, int fd ) :

Client::Client(const Client &src) {
	*this = src;
}

Client& Client::operator=(const Client &other) {
    if (this != &other) {
        server_ref = other.server_ref;
        location_ref = other.location_ref;
        client_addr = other.client_addr;
        len = other.len;
        socket_fd = other.socket_fd;
        resource_fd = other.resource_fd;
        resource_name = other.resource_name;
        status_code = other.status_code;
        requests = other.requests;
        reponse = other.reponse;
        start_connection = other.start_connection;
        no_request = other.no_request;
        current_time = other.current_time;
        empty_event = other.empty_event;
        bytes_sent = other.bytes_sent;
        emergency_overwrite = other.emergency_overwrite;
        is_directory = other.is_directory;
        is_cgi = other.is_cgi;
        finish_response = other.finish_response;
    }
    return *this;
}

Client::~Client( void ) { }

// Client&	Client::operator=( const Client &src ) {
// 	if (this == &src)
// 		return (*this);
// 	// server_ref = src.server_ref; socket_fd = src.socket_fd;
// 	// socket_addr = src.socket_addr; len = src.len; request_header = src.request_header;
// 	// request_body = src.request_body; response = src.response; bytes_sent = src.bytes_sent;
// 	// got_header = src.got_header; got_body = src.got_body; finish_response = src.finish_response;
// 	return (*this);
// }

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

void	Client::routeRequest( void ) {
	const HttpRequest &req = requests.front();
	if (!req.getValidHeader()) {
		status_code = 500;
	} else {
		server_ref->matchUri(*this);
	}
	if (status_code > 399) {
		;
	}
}

int	Client::findResource( void ) {
	if (finish_response) {
		return (-1);
	}
}

bool	Client::checkReponse( void ) {
	
}

const std::string&	Client::getCurrentUri( void ) const {
	return (requests.front().getUri());
}

void	Client::addResource( int status_code_, const std::string &str) {
	status_code = status_code_;
	resource_name = str;
}

void	Client::addDir( const std::string &str ) {
	resource_name = str;
	status_code = 200;
}

// std::vector<ServerInfo>::iterator	Client::getServerRef( void ) const {
// 	return(server_ref);
// }

bool	Client::checkHttpResponse( void ) const {
	if (status_code >= 300) {
		return (false);
	}
	return (true);
}

int	Client::clientSocketFd( void ) const { return (socket_fd); }

int	Client::getResourceFd( void ) const { return(resource_fd); }

std::ostream&	operator<<( std::ostream &o, const Client &ref ) {
	o << "Client socket fd: " << ref.clientSocketFd() << '\n';
	o << "Client reponse fd: " << ref.getResourceFd();// <<
		// ", reponse resource status: " << ((ref.isDataReady()) ? "ready" : "not ready") << '\n';
	// o << "Request status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	// o << "Request from Client\n" << ref.getRequest() << '\n';
	// o << "Reponse status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	// o << "Reponse to Client\n" << ref.getResponse() << '\n';
	return (o);
}

// size_t	Client::getBytesSent( void ) const { return(bytes_sent); }
