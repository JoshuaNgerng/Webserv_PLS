/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/07 22:03:26 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "AutoIndex.hpp"
#include "DefaultErrorPage.hpp"
#include "StaticFile.hpp"

Client::Client( void ) :
server_ref(),
location_ref(),
ignore_close_fd(false),
to_be_deleted(false),
socket_len(0),
socket_fd(-1),
content_name(),
content(NULL),
has_content_fd(true),
is_content_fd_in_server(false),
content_length(0),
status_code(0),
is_directory(false),
is_cgi(false),
is_proxy(false),
requests(),
response(),
start_connection(),
no_request(),
current_time(),
empty_event(),
bytes_sent(0),
emergency_overwrite(false)
{ }

Client::Client( server_ptr &it ) :
server_ref(it),
location_ref(it->getLocEnd()),
ignore_close_fd(false),
to_be_deleted(false),
socket_len(0),
socket_fd(-1),
content_name(),
content(),
has_content_fd(true),
is_content_fd_in_server(false),
content_length(0),
status_code(0),
is_directory(false),
is_cgi(false),
is_proxy(false),
requests(),
response(),
start_connection(),
no_request(),
current_time(),
empty_event(),
bytes_sent(0),
emergency_overwrite(false)
{ }

Client::Client( const Client &src ) {
	// std::cout << "client copy constructor called\n";
	*this = src;
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
	content_name = src.content_name;
	content = src.content;
	has_content_fd = src.has_content_fd;
	is_content_fd_in_server = src.is_content_fd_in_server;
	content_length = src.content_length;
	status_code = src.status_code;
	is_directory = src.is_directory;
	is_cgi = src.is_cgi;
	is_proxy = src.is_proxy;
	requests = src.requests;
	response = src.response;
	start_connection = src.start_connection;
	no_request = src.no_request;
	current_time = src.current_time;
	empty_event = src.empty_event;
	bytes_sent = src.bytes_sent;
	to_be_deleted = src.to_be_deleted;
	emergency_overwrite = src.emergency_overwrite;
	return (*this);
}

Client::~Client( void ) {
	std::cout << "client descrutor called\n";
	if (!ignore_close_fd) {
		if (socket_fd > 0) {
			std::cout << "close socket fd " << socket_fd << "\n";
			close(socket_fd);
		}
		if (content) {
			std::cout << "close content: " << *content << "\n";
			delete content;
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

void	Client::reset( void ) {
	if (requests.size() > 0)
		requests.pop();
	if (content) {
		delete content;
		content = NULL;
	}
	content_name.clear();
	has_content_fd = true;
	content_length = 0;
	status_code = 0;
	resetResponse();
	if (requests.size() > 0)
		routeRequest();
}

void	Client::resetResponse( void ) {
	is_directory = false;
	is_cgi = false;
	bytes_sent = 0;
	response.reset();
}

bool	Client::clientRecvHttp( void ) {
	char	buffer[buffer_size + 1];
	std::cout << "test socket_fd: " << socket_fd << '\n';
	ssize_t	r = recv(socket_fd, buffer, buffer_size, recv_flag);
	std::cout << "test client recv: " << r << '\n';
	if (r == 0) {
		return (false);//delete if socket fd is zero or less ?
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
	std::cout << "Queued request from: " << socket_fd << "\n" << requests.back();
	while (pos) {
		HttpRequest new_req;
		requests.push(new_req);
		str.erase(0, str.length() - pos);
		pos = requests.back().addRequest(str);
		std::cout << "Queued request from: " << socket_fd << "\n" << new_req;
	}
	routeRequest();
	// if (response_ready) {
		// std::cout << "Show HttpResponse\n" << response << '\n';
	// }
	return (true);
}

// true fd still active false fd not active remove from Server
bool	Client::clientRecvContent( void ) {
	char	buffer[buffer_size + 1];
	ssize_t	r = -1;
	if (is_proxy) {
		r = recv(content->getInputFd(), buffer, buffer_size, recv_flag);
	} else {
		r = read(content->getInputFd(), buffer, buffer_size);
	}
	if (r <= 0) {
		std::cout << "content fd: " << content->getInputFd() << " ";
		if (r == 0) {
			std::cout << "No bytes read: " << strerror(errno) << "\n";
		}
		else {
			std::cout << "client recv content close or failed: " << strerror(errno) <<"\n";
		}
		if (response.getBodyLength() != content_length) {
			std::cout << "what huh? length recieved " << response.getBodyLength() << " expected " << content_length << "\n";
			std::cout << response << '\n';
			status_code = 500;
			processResponseError();
		}
		return (false);
	}
	buffer[r] = '\0';
	response.addBody(buffer, static_cast<size_t>(r));
	if (response.getBodyLength() == content_length) {
		response.finishHttp();
		std::cout << "response for " << requests.front().getUri() << " is ready\n";
		// std::cout << "Show HttpResponse\n" << response << '\n';
		return (false);
	}
	else {
		std::cout << "response for " << requests.front().getUri() << " is not ready\n";
	}
	return (true);
}

bool	Client::clientSendResponse( void ) {
	// std::cout << "client Send Response\n" << response;
	ssize_t	no_bytes = send(socket_fd,
		response.getPtr2Http(bytes_sent), response.getTotalLength() - bytes_sent, 0);
	if (no_bytes <= 0) {
		std::cout << "error sending to client\n";
		reset();
		return (false);
	}
	bytes_sent += no_bytes;
	if (bytes_sent != response.getTotalLength()) {
		// std::cout << response << "\nSent Response to: " << socket_fd << "\n";
		// reset();
		return (true);
	}
	return (false);
}

void	Client::processResponseSuccess( void ) {
	if (is_directory) {
		const InfoBlock	*ptr = &(*server_ref);
		if (location_ref != server_ref->getLocEnd() && location_ref->getAutoIndex() == on) {
			ptr = &(*location_ref);
		}
		AutoIndex	gen(*ptr);
		std::string	path(root_dir + content_name);
		response.addBody(gen.generateResource(path));
		content_length = response.getBodyLength();
		response.setHeader(status_code);
		response.setContent(Http::getMimeType(gen.getExtension()), content_length);
		return ;
	}
	const char *ext = CheckFile::fetchExtension(content_name);
	// check if cgi, is_cgi = true;
	if (!processContent()) {
		processResponseError();
		return ;
	}
	response.setHeader(status_code);
	response.setContent(Http::getMimeType(ext), content_length);
}

void	Client::processResponseRedirect( void ) {
	std::cout << "Redirect " << status_code << '\n';
	has_content_fd = false;
	response.setHeader(status_code, content_name);
	response.setContent();
	response.finishHttp();
}

void	Client::getDefaultError( void ) {
	std::cout << "getting default error " << status_code << '\n';
	response.addBody(DefaultErrorPage::generateHtml(status_code, Server::server_name));
	content_length = response.getBodyLength();
	response.setContent(Http::getMimeType("html"), content_length);
	response.finishHttp();
	has_content_fd = false;
}

bool	Client::processContent( void ) {
	std::string path = root_dir + content_name;
	return (processContent(path));
}

bool	Client::processContent( const std::string &path ) {
	// do checking for cgi proxy
	File *ptr = NULL;
	ptr = new StaticFile();
	ptr->setContentPath(path);
	if (!ptr->processFds()) {
		delete ptr;
		return (false);
	}
	content = ptr;
	return (true);
}


void	Client::processResponseError( void ) {
	resetResponse();
	std::cout << "error status_code " << status_code << '\n';
	content_name.clear();
	response.setHeader(status_code);
	if (location_ref != server_ref->getLocEnd()) {
		location_ref->findErrorPath(content_name, status_code);
	}
	if (!content_name.length()) {
		server_ref->findErrorPath(content_name, status_code);
	}
	// std::cout << "client process error\n";
	if (!content_name.length()) {
		getDefaultError();
		return ;
	}
	std::string	path = root_dir + content_name; 
	CheckFile	check(path);
	check.checking(F_OK | R_OK);
	if (check.getAccessbility() < 0) {
		getDefaultError();
		return ;
	}
	if (content) {
		delete content;
		content = NULL;
	}
	if (!processContent()) {
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
	int	check = req.getValidHeader();
	if (check > 0) {
		// std::cout << "bad req for invalid header\n";
		status_code = check;
		emergency_overwrite = true;
		processResponseError();
		return ;
	} 
	if (server_ref->isMergeSlash()) {
		req.normalizeUri();
	}
	server_ref->routingClient(*this);
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

void	Client::addRootDir( const std::string &str ) {
	root_dir = str;
}

void	Client::addRootDir( const std::string &root, const std::string &uri ) {
	root_dir = root + uri;
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

bool	Client::checkResponseReady( void ) const { return (response.isReady()); }

bool	Client::giveContentFdtoServer( void ) const {
	if (!content) {
		return (false);
	}
	return (content->getFds());
}

bool	Client::toBeDeleted( void ) const { return (to_be_deleted); }

void	Client::ignoreClosingFd( void ) { ignore_close_fd = true; }

void	Client::serverReceived( void ) { content->serverReceivedFds(); }

int	Client::clientSocketFd( void ) const { return (socket_fd); }

std::string	Client::getAddr( void ) const {
	char ip_str[INET6_ADDRSTRLEN];
	if (client_addr.ss_family == AF_INET) {
		struct sockaddr_in* addr_in = (struct sockaddr_in*)&client_addr;
		inet_ntop(AF_INET, &addr_in->sin_addr, ip_str, sizeof(ip_str));
		unsigned short port = ntohs(addr_in->sin_port);
	}
	else if (client_addr.ss_family == AF_INET6) {
		struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)&client_addr;
		inet_ntop(AF_INET6, &addr_in6->sin6_addr, ip_str, sizeof(ip_str));
		unsigned short port = ntohs(addr_in6->sin6_port);
	}
	return (std::string(ip_str));
}

uint16_t	Client::getPort( void ) const {
	if (client_addr.ss_family == AF_INET) {
		struct sockaddr_in* addr_in = (struct sockaddr_in*)&client_addr;
		return (ntohs(addr_in->sin_port));
	}
	struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)&client_addr;
	return (ntohs(addr_in6->sin6_port));
}

const std::string&	Client::getRoot( void ) const { return (root_dir); }

const std::string&	Client::getCurrentUri( void ) const { return (requests.front().getUri()); }

const std::string&	Client::getCurrentUrl( void ) const { return(requests.front().getUrl()); }

const std::string&	Client::getCurrentPath( void ) const { return(requests.front().getPath()); }

const std::string&	Client::getCurrentQuery( void ) const { return(requests.front().getQuery()); }

std::string	Client::getContentType( void ) const { return (requests.front().getContentType()); }

size_t	Client::getContentLength( void ) const { return(requests.front().getContentLength()); }

const std::string&	Client::getHost( void ) const { return(requests.front().getField("host")); }

std::string	Client::getReqMethod( void ) const {
	return (requests.front().getContentType());
}

std::string	Client::getHttpScheme( void ) const { return("http"); }

const HttpRequest&	Client::getCurrentHttpRequest( void ) const { return(requests.front()); }

const File*	Client::getContent( void ) const { return(content); }

std::ostream&	operator<<( std::ostream &o, const Client &ref ) {
	o << "Client socket fd: " << ref.clientSocketFd() << '\n';
	o << "Client response fd: " << *(ref.getContent()) << '\n';
	// << ", response content status: " << ((ref.isDataReady()) ? "ready" : "not ready") << '\n';
	// o << "Request status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	// o << "Request from Client\n" << ref.getRequest() << '\n';
	// o << "response status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	// o << "response to Client\n" << ref.getResponse() << '\n';
	return (o);
}

