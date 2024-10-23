/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/22 23:34:04 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "AutoIndex.hpp"
#include "DefaultErrorPage.hpp"

Client::Client( void ) : socket_fd(-1), content_fd(-1) { }

Client::Client( server_ptr &it ) :
	server_ref(it), location_ref(it->getLocEnd()), socket_fd(-1), content_fd(-1) { }

Client::Client( const Client &src ) {
	*this = src;
}

Client& Client::operator=( const Client &src ) {
	if (this != &src) {
		server_ref = src.server_ref;
		location_ref = src.location_ref;
		client_addr = src.client_addr;
		len = src.len;
		socket_fd = src.socket_fd;
		content_fd = src.content_fd;
		content_name = src.content_name;
		has_content_fd = src.has_content_fd;
		length = src.length;
		status_code = src.status_code;
		requests = src.requests;
		response = src.response;
		start_connection = src.start_connection;
		is_directory = src.is_directory;
		is_cgi = src.is_cgi;
		finish_response = src.finish_response;
		no_request = src.no_request;
		current_time = src.current_time;
		empty_event = src.empty_event;
		bytes_sent = src.bytes_sent;
		emergency_overwrite = src.emergency_overwrite;
		completed = src.completed;
	}
	return (*this);
}

Client::~Client( void ) {
	if (socket_fd > 0) {
		close(socket_fd);
	}
	if (content_fd > 0) {
		close(content_fd);
	}
}

Client&	Client::operator=( const std::vector<Location>::const_iterator &it ) {
	location_ref = it;
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

void	Client::reset( void ) {
	if (requests.size() > 0)
		requests.pop();
	response.reset();
	if (content_fd > 0)
		close(content_fd);
	content_fd = -1;
	content_name.clear();
	has_content_fd = true;
	length = 0;
	status_code = 0;
	is_directory = false;
	is_cgi = false;
	finish_response = false;
}

bool	Client::clientRecvHttp( void ) {
	char	buffer[recv_buffer_size + 1];
	ssize_t	r = recv(socket_fd, buffer, recv_buffer_size, recv_flag);
	if (r == 0) {
		return (false);
	}
	if (r < 0) {
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
	routeRequest();
	return (true);
}

bool	Client::clientRecvContent( void ) {
	char	buffer[recv_buffer_size + 1];
	ssize_t	r = recv(socket_fd, buffer, recv_buffer_size, recv_flag);
	if (r == 0 || r < 0) {
		if (response.getBodyLength() != length) {
			if (status_code == 500) {
				processResponseError(); // deafault html
				return (false);
			}
			status_code = 500;
			processResponseError();
		}
		return (false);
	}
	buffer[r] = '\0';
	response.addBody(buffer);
	if (response.getBodyLength() == length) {
		response.finishResponseMsg();
		length = response.getTotalLength();
		return (false);
	}
	return (true);
}

bool	Client::clientSendResponse( void ) {
	ssize_t	no_bytes = send(socket_fd,
		response.getPtrPos(bytes_sent), length - bytes_sent, 0);
	if (no_bytes <= 0) {
		return (false);
	}
	bytes_sent += no_bytes;
	if (bytes_sent == length) {
		reset();
	}
	return (true);
}

void	Client::processResponseSuccess( void ) {
	response.setHeader(status_code, content_name);
	if (is_directory) {
		const InfoBlock	*ptr = &(*server_ref);
		if (location_ref != server_ref->getLocEnd() && location_ref->getAutoIndex() == on) {
			ptr = &(*location_ref);
		}
		AutoIndex	gen(*ptr);
		response.addBody(gen.generateResource(content_name));
		length = response.getBodyLength();
		response.setContent(Http::getMimeType(gen.getExtension()), length);
		return ;
	}
	const char *ext = CheckFile::fetchExtension(content_name);
	// check if cgi
	// is_cgi = true;
	content_fd = open(content_name.c_str(), O_RDONLY);
	if (content_fd < 0) {
		status_code = 500;
		processResponseError();
		return ;
	}
	if (fcntl(content_fd, F_SETFL, O_NONBLOCK) < 0) {
		close(content_fd);
		content_fd = -1;
		status_code = 500;
		processResponseError();
		return ;
	}
	response.setContent(Http::getMimeType(ext), length);
}

void	Client::processResponseRedirect( void ) {
	has_content_fd = false;
	response.setHeader(status_code, content_name);
}

void	Client::processResponseError( void ) {
	response.setHeader(status_code, content_name);
	if (location_ref == server_ref->getLocEnd()) {
		server_ref->findErrorPath(content_name, status_code);
	}
	else {
		location_ref->findErrorPath(content_name, status_code);
		if (!content_name.length())
			server_ref->findErrorPath(content_name, status_code);
	}
	if (content_name.length()) {
		CheckFile	check(content_name);
		check.checking(F_OK | R_OK);
		if (check.getAccessbility() < 0) {
			goto default_html;
			return ;
		}
		has_content_fd = true;
		response.setContent(
			Http::getMimeType(CheckFile::fetchExtension(content_name)),
			check.getFilesize()
		);
		return ;
	}
	default_html:
	response.addBody(DefaultErrorPage::generateHtml(status_code, Server::server_name));
	length = response.getBodyLength();
	response.setContent(Http::getMimeType("html"), length);
}

void	Client::routeRequest( void ) {
	HttpRequest &req = requests.front();
	if (server_ref->isMergeSlash()) {
		req.normalizeUri();
	}
	if (!req.getValidHeader()) {
		status_code = 400;
		emergency_overwrite = true;
		processResponseError();
		return ;
	} 
	server_ref->matchUri(*this);
	if (status_code > 199 && status_code < 300) {
		processResponseSuccess();
		return ;
	}
	if (status_code > 299 && status_code < 400) {
		processResponseRedirect();
		return ;
	}
	processResponseError();
}

const std::string&	Client::getCurrentUri( void ) const {
	return (requests.front().getUri());
}

void	Client::addContent( int status_code_, const std::string &str, size_t content_length ) {
	status_code = status_code_;
	content_name = str;
	length = content_length;
}

void	Client::addDir( const std::string &str ) {
	content_name = str;
	is_directory = true;
	status_code = 200;
}

std::vector<ServerInfo>::const_iterator	Client::getServerRef( void ) const {
	return (server_ref);
}

std::vector<Location>::const_iterator	Client::getLocationRef( void ) const {
	return (location_ref);
}

bool	Client::checkHttpResponse( void ) const {
	if (status_code >= 300) {
		return (false);
	}
	return (true);
}

int	Client::clientSocketFd( void ) const { return (socket_fd); }

int	Client::getContentFd( void ) const { return(content_fd); }

std::ostream&	operator<<( std::ostream &o, const Client &ref ) {
	o << "Client socket fd: " << ref.clientSocketFd() << '\n';
	o << "Client response fd: " << ref.getContentFd();// <<
		// ", response content status: " << ((ref.isDataReady()) ? "ready" : "not ready") << '\n';
	// o << "Request status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	// o << "Request from Client\n" << ref.getRequest() << '\n';
	// o << "response status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	// o << "response to Client\n" << ref.getResponse() << '\n';
	return (o);
}

bool	Client::checkResponseStatus( void ) const { return (finish_response); }

// size_t	Client::getBytesSent( void ) const { return(bytes_sent); }
