/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/08 03:36:45 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "AutoIndex.hpp"
#include "DefaultErrorPage.hpp"

Client::Client( void ) : socket_fd(-1), content_fd(-1) { }

Client::Client( server_ptr &it ) :
	socket_fd(-1), content_fd(-1), server_ref(it), location_ref(it->getLocEnd()) { }

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
		fetch_content = src.fetch_content;
		length = src.length;
		status_code = src.status_code;
		requests = src.requests;
		reponse = src.reponse;
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

bool	Client::clientRecv( void ) {
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
	return (true);
}

bool	Client::fetchContentFd( void ) {
	if (is_cgi) {
		return (true);
	}
	content_fd = open(content_name.c_str(), O_RDONLY);
	if (content_fd < 0)
		return (false);
	if (fcntl(content_fd, F_SETFL, O_NONBLOCK) < 0) {
		close(content_fd);
		content_fd = -1;
		return (false);
	}
	return (true);

}

void	Client::processReponseSucess( void ) {
	if (is_directory) {
		AutoIndex	gen(server_ref->getAutoFormat(), server_ref->getAutoTimeFormat(), server_ref->getAutoSize());
		reponse.addBody(gen.generateResource(content_name));
		length = reponse.getBodyLength();
		reponse.setContent("", length);
		return ;
	}
	// check if cgi
	// is_cgi = true;
	content_fd = open(content_name.c_str(), O_RDONLY);
	if (content_fd < 0) {
		status_code = 500;
		processReponseError();
		return ;
	}
	if (fcntl(content_fd, F_SETFL, O_NONBLOCK) < 0) {
		close(content_fd);
		content_fd = -1;
		status_code = 500;
		processReponseError();
		return ;
	}
	reponse.setContent("", length);
}

void	Client::processReponseRedirect( void ) {
	fetch_content = false;
	reponse.setHeader(status_code, content_name);
}

void	Client::processReponseError( void ) {
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
		fetch_content = true;
		reponse.setContent("", check.getFilesize());
		return ;
	}
	default_html:
	reponse.addBody(DefaultErrorPage::generateHtml(status_code, Server::server_name));
	length = reponse.getBodyLength();
	reponse.setContent("", length);
}

void	Client::routeRequest( void ) {
	HttpRequest &req = requests.front();
	if (server_ref->isMergeSlash()) {
		req.normalizeUri();
	}
	if (!req.getValidHeader()) {
		status_code = 400;
		emergency_overwrite = true;
		processReponseError();
		return ;
	} 
	server_ref->matchUri(*this);
	if (status_code > 199 && status_code < 300) {
		processReponseSucess();
		return ;
	}
	if (status_code > 299 && status_code < 400) {
		processReponseRedirect();
		return ;
	}
	processReponseError();
}

int	Client::getContentFd( void ) const { return (content_fd); }

bool	Client::checkReponse( void ) {
	
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
	o << "Client reponse fd: " << ref.getContentFd();// <<
		// ", reponse content status: " << ((ref.isDataReady()) ? "ready" : "not ready") << '\n';
	// o << "Request status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	// o << "Request from Client\n" << ref.getRequest() << '\n';
	// o << "Reponse status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	// o << "Reponse to Client\n" << ref.getResponse() << '\n';
	return (o);
}

// size_t	Client::getBytesSent( void ) const { return(bytes_sent); }
