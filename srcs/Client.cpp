/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/30 20:04:27 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "AutoIndex.hpp"
#include "DefaultErrorPage.hpp"

Client::Client( void ) :
server_ref(),
location_ref(),
ignore_close_fd(false),
socket_len(0),
socket_fd(-1),
content_fd(-1),
has_content_fd(true),
is_content_fd_in_server(false),
content_length(0),
status_code(0),
is_directory(false),
is_cgi(false),
response_ready(false),
requests(),
response(),
start_connection(),
no_request(),
current_time(),
empty_event(),
bytes_sent(0),
emergency_overwrite(false),
completed(false),
to_be_deleted(false)
{ }

Client::Client( server_ptr &it ) :
server_ref(it),
location_ref(it->getLocEnd()),
ignore_close_fd(true),
socket_len(0),
socket_fd(-1),
content_fd(-1),
has_content_fd(true),
is_content_fd_in_server(false),
content_length(0),
status_code(0),
is_directory(false),
is_cgi(false),
response_ready(false),
requests(),
response(),
start_connection(),
no_request(),
current_time(),
empty_event(),
bytes_sent(0),
emergency_overwrite(false),
completed(false),
to_be_deleted(false)
{ }

Client::Client( const Client &src ) {
	std::cout << "client copy constructor called\n";
	*this = src;
	ignore_close_fd = false;
}

Client&	Client::operator=( const Client &src ) {
	if (this == &src) {
		return (*this);
	}
	server_ref = src.server_ref;
	location_ref = src.location_ref;
	ignore_close_fd = src.ignore_close_fd;
	client_addr = src.client_addr;
	socket_len = src.socket_len;
	socket_fd = src.socket_fd;
	content_fd = src.content_fd;
	content_name = src.content_name;
	has_content_fd = src.has_content_fd;
	is_content_fd_in_server = src.is_content_fd_in_server;
	content_length = src.content_length;
	status_code = src.status_code;
	is_directory = src.is_directory;
	is_cgi = src.is_cgi;
	response_ready = src.response_ready;
	requests = src.requests;
	response = src.response;
	start_connection = src.start_connection;
	no_request = src.no_request;
	current_time = src.current_time;
	empty_event = src.empty_event;
	bytes_sent = src.bytes_sent;
	emergency_overwrite = src.emergency_overwrite;
	completed = src.completed;
	to_be_deleted = src.to_be_deleted;
	return (*this);
}

Client::~Client( void ) {
	std::cout << "client descrutor called\n";
	if (!ignore_close_fd) {
		if (socket_fd > 0) {
			std::cout << "close socket fd\n";
			close(socket_fd);
		}
		if (content_fd > 0) {
			std::cout << "close content fd\n";
			close(content_fd);
		}
	}
}

Client&	Client::operator<<( const std::vector<Location>::const_iterator &it ) {
	location_ref = it;
	return (*this);
}

int	Client::clientSocketFd( int listen_fd ) {
	socket_len = sizeof(client_addr);
	socket_fd = accept(listen_fd, (sockaddr_t *)(&client_addr), &socket_len);
	if (socket_fd < 0) {
		return (-1);
	}
	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0) {
		close(socket_fd);
		return (-1);
	}
	return (socket_fd);
}

int	Client::getStaticFileFd( void ) {
	return (content_fd = open(content_name.c_str(), O_RDONLY));
}

bool	Client::getCgiPipeFd( void ) {
	int	pipe_fd[2];
	if (content_fd > 0) {
		close(content_fd);
	}
	content_fd = -1;
	if (pipe(pipe_fd) < 0) {
		return (false);
	}
	content_fd = pipe_fd[1];
	// int pid = fork();
	close(pipe_fd[0]);
	if (fcntl(content_fd, F_SETFL, O_NONBLOCK) < 0) {
		close(content_fd);
		content_fd = -1;
		status_code = 500;
		return (false);
	}
	return (true);
}

bool	Client::getProxySocketFd( void ) {
	if (content_fd > 0) {
		close(content_fd);
	}
	
	if (fcntl(content_fd, F_SETFL, O_NONBLOCK) < 0) {
		close(content_fd);
		content_fd = -1;
		status_code = 500;
		return (false);
	}
	return (true);
}

bool	Client::processContentFd( int (Client::*func)( void ) ) {
	if (content_fd > 0) {
		close(content_fd);
	}
	content_fd = (this->*func)();
	if (content_fd < 0) {
		status_code = 500;
		return (false);
	}
	if (fcntl(content_fd, F_SETFL, O_NONBLOCK) < 0) {
		close(content_fd);
		content_fd = -1;
		status_code = 500;
		return (false);
	}
	return (true);	
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
	content_length = 0;
	status_code = 0;
	is_directory = false;
	is_cgi = false;
	response_ready = false;
}

bool	Client::clientRecvHttp( void ) {
	char	buffer[recv_buffer_size + 1];
	ssize_t	r = recv(socket_fd, buffer, recv_buffer_size, recv_flag);
	if (r == 0) {
		return (false);//delete if socket fd is zero or less ?
	}
	if (r < 0) {
		return (false);
	}
	buffer[r] = '\0';
	std::string	str(buffer);
	std::cout << "testing recv http\n" << str ;
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

// true fd still active false fd not active remove from Server
bool	Client::clientRecvContent( void ) {
	char	buffer[recv_buffer_size + 1];
	ssize_t	r = recv(socket_fd, buffer, recv_buffer_size, recv_flag);
	if (r == 0 || r < 0) {
		if (response.getBodyLength() != content_length) {
			status_code = 500;
			processResponseError();
		}
		return (false);
	}
	buffer[r] = '\0';
	response.addBody(buffer);
	if (response.getBodyLength() == content_length) {
		response.finishResponseMsg();
		response_ready = true;
		return (false);
	}
	return (true);
}

bool	Client::clientSendResponse( void ) {
	ssize_t	no_bytes = send(socket_fd,
		response.getPtrPos(bytes_sent), response.getTotalLength() - bytes_sent, 0);
	if (no_bytes <= 0) {
		return (false);
	}
	bytes_sent += no_bytes;
	if (bytes_sent == response.getTotalLength()) {
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
		content_length = response.getBodyLength();
		response.setContent(Http::getMimeType(gen.getExtension()), content_length);
		return ;
	}
	const char *ext = CheckFile::fetchExtension(content_name);
	// check if cgi, is_cgi = true;
	if (!processContentFd(&Client::getStaticFileFd)) {
		processResponseError();
		return ;
	}
	response.setContent(Http::getMimeType(ext), content_length);
}

void	Client::processResponseRedirect( void ) {
	has_content_fd = false;
	response.setHeader(status_code, content_name);
}

void	Client::getDefaultError( void ) {
	response.addBody(DefaultErrorPage::generateHtml(status_code, Server::server_name));
	content_length = response.getBodyLength();
	response.setContent(Http::getMimeType("html"), content_length);
	response_ready = true;
	has_content_fd = false;
}

void	Client::processResponseError( void ) {
	response.setHeader(status_code, content_name);
	content_name.clear();
	if (location_ref != server_ref->getLocEnd()) {
		location_ref->findErrorPath(content_name, status_code);
	}
	if (!content_name.length())
		server_ref->findErrorPath(content_name, status_code);
	if (!content_name.length()) {
		getDefaultError();
		return ;
	}
	CheckFile	check(content_name);
	check.checking(F_OK | R_OK);
	if (check.getAccessbility() < 0) {
		getDefaultError();
		return ;
	}
	if (content_fd > 0) {
		close(content_fd);
	}
	if (!processContentFd(&Client::getStaticFileFd)) {
		getDefaultError();
		return ;
	}
	has_content_fd = true;
	is_content_fd_in_server = false;
	response.setContent(
		Http::getMimeType(CheckFile::fetchExtension(content_name)),
		check.getFilesize()
	);
}

void	Client::errorOverwriteResponse( void ) {
	emergency_overwrite = true;
	getDefaultError();
}

void	Client::errorOverwriteResponse( int status ) {
	status_code = status;
	if (emergency_overwrite) {
		processResponseError();
		emergency_overwrite = true;
		return ;
	}
	getDefaultError();
}

void	Client::markforDeletion( void ) { to_be_deleted = true; }

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

void	Client::addContent( int status_code_, const std::string &str, size_t content_length_ ) {
	status_code = status_code_;
	content_name = str;
	content_length = content_length_;
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

bool	Client::checkResponseStatus( void ) const {
	return ((status_code < 300) ? true : false);
}

bool	Client::checkResponseReady( void ) const { return (response_ready); }

bool	Client::giveContentFdtoServer( void ) const {
	return (is_content_fd_in_server);
}

bool	Client::toBeDeleted( void ) const { return (to_be_deleted); }

void	Client::serverReceived( void ) { is_content_fd_in_server = true; }

int	Client::clientSocketFd( void ) const { return (socket_fd); }

int	Client::getContentFd( void ) const { return(content_fd); }

std::ostream&	operator<<( std::ostream &o, const Client &ref ) {
	o << "Client socket fd: " << ref.clientSocketFd() << '\n';
	o << "Client response fd: " << ref.getContentFd() << '\n';
	// << ", response content status: " << ((ref.isDataReady()) ? "ready" : "not ready") << '\n';
	// o << "Request status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	// o << "Request from Client\n" << ref.getRequest() << '\n';
	// o << "response status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	// o << "response to Client\n" << ref.getResponse() << '\n';
	return (o);
}

