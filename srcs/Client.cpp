/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/15 03:12:49 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "AutoIndex.hpp"
#include "DefaultErrorPage.hpp"
#include "StaticFile.hpp"
#include "CgiPipe.hpp"

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
start_connection(0),
current_time(0),
emergency_overwrite(false),
body_timeout(0),
start_recv_body(0),
start_recv_cgi(0)
{ }

Client::Client( server_ptr &it ) :
server_ref(it),
location_ref(it->getLocEnd()),
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
start_connection(0),
current_time(0),
emergency_overwrite(false),
body_timeout(0),
start_recv_body(0),
start_recv_cgi(0)
{ }

Client::Client( const Client &src ) {
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
	current_time = src.current_time;
	to_be_deleted = src.to_be_deleted;
	emergency_overwrite = src.emergency_overwrite;
	body_timeout = src.body_timeout;
	start_recv_body = src.start_recv_body;
	start_recv_cgi = src.start_recv_cgi;
	return (*this);
}

Client::~Client( void ) {
	if (!ignore_close_fd) {
		if (socket_fd > 0) {
			close(socket_fd);
		}
		if (content) {
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

void	Client::detachContent( void ) { content = NULL; }

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
	if (requests.size() > 0) {
		if (requests.front().isHeaderReady())
			routeRequest();
	}
	is_cgi = false;
	start_connection = 0;
	current_time = 0;
	start_recv_body = 0;
	start_recv_cgi = 0;
}

void	Client::resetResponse( void ) {
	is_directory = false;
	is_cgi = false;
	response.reset();
	emergency_overwrite = false;
}

bool	Client::clientRecvHttp( void ) {
	char	buffer[buffer_size + 1];
	ssize_t	r = recv(socket_fd, buffer, buffer_size, recv_flag);
	if (r == 0) {
		return (false);//delete if socket fd is zero or less ?
	}
	if (r < 0) {
		return (false);
	}
	buffer[r] = '\0';
	std::string	str;
	str.append(buffer, r);
	if (!requests.size() || requests.back().isReady()) {
		HttpRequest new_req(server_ref->getHeaderSizeLimit());
		requests.push(new_req);
	}
	size_t pos = requests.back().addRequest(str);
	while (pos) {
		if (requests.back().isReady()) {
			HttpRequest new_req(server_ref->getHeaderSizeLimit());
			requests.push(new_req);
		}
		str.erase(0, str.length() - pos);
		pos = requests.back().addRequest(str);
	}
	int	check = requests.front().getValidHeader();
	if (check > 0) {
		routeError(check);
	} else if (requests.front().isReady()) {
		routeRequest();
	}
	if (requests.front().isHeaderReady()) {
		setStartBodyTime();	
	}
	return (true);
}

bool	Client::clientRecvContent( void ) {
	if (!content || !content->checkStatus()) {
		return (false);
	}
	if (is_proxy) {  } // r = recv(content->getInputFd(), buffer, buffer_size, recv_flag);
	if (is_cgi) {
		return (clientRecvCgi());;
	}
	return (clientRecvStaticFile());
}

bool	Client::clientRecvStaticFile( void ) {
	char	buffer[buffer_size + 1];
	ssize_t	r = read(content->getInputFd(), buffer, buffer_size);
	if (r < 0) {
		return (false);
	} else if (r == 0) {
		if (response.getBodyLength() != content_length) {
			status_code = 500;
			processResponseError();
		}
		return (false);
	}
	buffer[r] = '\0';
	response.addBody(buffer, static_cast<size_t>(r));
	if (response.getBodyLength() == content_length) {
		response.finishHttp();
		return (false);
	}
	return (true);
}

bool	Client::clientRecvCgi( void ) {
	char	buffer[buffer_size + 1];
	ssize_t r = read(content->getInputFd(), buffer, buffer_size);
	if (r < 0) {
		return (false);
	} else if (r == 0) {
		response.processCgiData();
		return (false);
	}
	buffer[r] = '\0';
	response.addFin(buffer, static_cast<size_t>(r));
	response.processCgiDataHeader();
	if (response.isHeaderReady() && response.getContentLength()) {
		if (response.getTotalLength() >= response.getContentLength()) {
			response.processCgiData();
			return (false);
		}
	}
	return (true);
}

bool	Client::clientSendContent( void ) {
	if (!content || !content->checkStatus()) {
		return (false);
	}
	return (clientSendCgi());
}

bool	Client::clientSendCgi( void ) {
	HttpRequest	&req = requests.front();
	ssize_t	no_bytes = write(content->getOutputFd(),
		req.getPtr2Body(), req.getRemainderBody());
	if (no_bytes <= 0) {
		return (false);
	}
	if (!req.checkSendBody(no_bytes)) {
		return (true);
	}
	return (false);	
}

bool	Client::clientSendResponse( void ) {
	ssize_t	no_bytes = send(socket_fd,
		response.getPtr2Http(), response.getRemainderHttp(), 0);
	if (no_bytes <= 0) {
		reset();
		return (false);
	}
	if (!response.checkSendHttp(no_bytes)) {
		return (true);
	}
	return (false);
}

bool	Client::checkContentStatus( void ) {
	if (is_cgi) {
		if (!content->checkStatus()) {
			return (false);
		}
		if (!response.getTotalLength()) {
			response.setHeader(200);
			response.setContent();
			response.finishHttp();
		} else {
			response.processCgiData();
		}
		return (true);
	}
	if (response.getBodyLength() != content_length) {
		return (false);
	} else {
		if (!response.isReady()) {
			response.finishHttp();
		}
	}
	return (true);
}

void	Client::processResponseSuccess( void ) {
	if (is_directory) {
		const InfoBlock	*ptr = &(*server_ref);
		if (location_ref != server_ref->getLocEnd() && location_ref->getAutoIndex() == on) {
			ptr = &(*location_ref);
		}
		AutoIndex	gen(*ptr);
		gen.getUriFromClient(*this);
		response.addBody(gen.generateResource(content_name));
		content_length = response.getBodyLength();
		response.setHeader(status_code);
		response.setContent(content_length, Http::getMimeType(gen.getExtension()));
		response.finishHttp();
		return ;
	}
	if (!processContent()) {
		processResponseError();
	}
}

void	Client::processResponseRedirect( void ) {
	has_content_fd = false;
	response.setHeader(status_code, content_name);
	response.setContent();
	response.finishHttp();
}

void	Client::getDefaultError( void ) {
	response.reset();
	response.addBody(DefaultErrorPage::generateHtml(status_code, Server::server_name));
	content_length = response.getBodyLength();
	response.setHeader(status_code);
	response.setContent(content_length, Http::getMimeType("html"));
	response.finishHttp();
	has_content_fd = false;
	if (content) {
		delete content;
	}
	content = NULL;
}

File*	Client::processContentCgiHelper( const char* ext ) {
	bool	cgi_check = false;
	std::string	bin, check;
	if (status_code > 299) {
		response.setHeader(status_code);
		response.setContent(content_length, Http::getMimeType(ext));
		return (new StaticFile());
	}
	if (location_ref != server_ref->getLocEnd()) {
		cgi_check = location_ref->isCgi(ext);
	}
	if (!cgi_check) {
		cgi_check = server_ref->isCgi(ext);
	}
	if (!cgi_check) {
		response.setHeader(status_code);
		response.setContent(content_length, Http::getMimeType(ext));
		return (new StaticFile());
	}
	is_cgi = true;
	if (location_ref != server_ref->getLocEnd()) {
		bin = location_ref->getCgiBin(ext);
	}
	if (!bin.length()) {
		bin = server_ref->getCgiBin(ext);
	}
	return (new CgiPipe(bin, *this));
}

bool	Client::processContent( void ) {
	std::string path = root_dir;
	if (content_name[0] != '/') {
		path += "/";
	}
	path += content_name;
	return (processContent(path));
}

bool	Client::processContent( const std::string &path ) {
	File		*ptr = NULL;
	const char	*ext = CheckFile::fetchExtension(content_name);
	if (content) {
		delete content;
		content = NULL;
	}
	ptr = processContentCgiHelper(ext);
	ptr->setContentPath(path);
	if (!ptr->processFds()) {
		delete ptr;
		status_code = 500;
		return (false);
	}
	content = ptr;
	return (true);
}

void	Client::processResponseError( void ) {
	resetResponse();
	content_name.clear();
	if (location_ref != server_ref->getLocEnd()) {
		location_ref->findErrorPath(content_name, status_code);
	} else {
		server_ref->findErrorPath(content_name, status_code);
	}
	if (!content_name.length()) {
		getDefaultError();
		return ;
	}
	std::string	path = root_dir + '/';
	path += content_name; 
	CheckFile	check(path);
	check.checking(F_OK | R_OK);
	if (check.getAccessbility() < 0) {
		getDefaultError();
		return ;
	}
	content_length = check.getFilesize();
	if (!processContent()) {
		getDefaultError();
		return ;
	}
	has_content_fd = true;
	is_content_fd_in_server = false;
}

void	Client::errorOverwriteResponse( void ) {
	emergency_overwrite = true;
	resetResponse();
	getDefaultError();
}

void	Client::errorOverwriteResponse( int status ) {
	if (status_code == status) {
		emergency_overwrite = true;
		errorOverwriteResponse();
		return ;
	}
	status_code = status;
	if (!emergency_overwrite) {
		processResponseError();
		emergency_overwrite = true;
		return ;
	}
	errorOverwriteResponse();
}

void	Client::markforDeletion( void ) { to_be_deleted = true; }

void	Client::routeError( int status ) {
	status_code = status;
	emergency_overwrite = true;
	processResponseError();
}

bool	Client::checkBodySize( const HttpRequest &req ) {
	size_t	limit = server_ref->getBodySizeLimit();
	if (location_ref != server_ref->getLocEnd()) {
		limit = location_ref->getBodySizeLimit();
	}
	if (req.getContentLength() < limit) {
		return (true);
	}
	return (false);
}

void	Client::routeRequest( void ) {
	HttpRequest &req = requests.front();
	if (server_ref->isMergeSlash()) {
		req.normalizeUri();
	}
	server_ref->routingClient(*this); // start body timeout and check size
	if (!checkBodySize(req)) {
		status_code = 413;
	}
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

void	Client::serverReceived( void ) { content->serverReceivedFds(); setTimeForContent(); }

void	Client::setStartConnectionTime( void ) { start_connection = std::time(NULL); }

void	Client::setStartBodyTime( void ) { start_recv_body = std::time(NULL); }

void	Client::setTimeForContent( void ) { start_recv_cgi = std::time(NULL); }

void	Client::setNewCurrentTime( void ) { current_time = std::time(NULL); }

int	Client::checkTimer( int fd ) {
	size_t	time_diff = 0, lim = 0;
	int		status = 200;
	if (fd == socket_fd) {
		if (!requests.size()) {
			return (200);
		}
		HttpRequest &req = requests.front();
		if (req.isReady()) {
			return (200);
		}
		status = 408;
		if (!req.isHeaderReady()) {
			time_diff = static_cast<size_t>(
				std::difftime(current_time, start_connection)
			);
			lim = server_ref->getHeaderTimeout();
		} else {
			time_diff = static_cast<size_t>(
				std::difftime(current_time, start_recv_body)
			);
			lim = server_ref->getBodyTimeout();
			if (location_ref != server_ref->getLocEnd()) {
				lim = location_ref->getBodyTimeout();
			}
		}
	} else {
		status = 504;
		time_diff = static_cast<size_t>(
			std::difftime(current_time, start_recv_cgi)
		);
		lim = server_ref->getCgiTimeout();
		if (location_ref != server_ref->getLocEnd()) {
			lim = location_ref->getCgiTimeout();
		}
	}
	if (time_diff > lim) {
		return (status);
	}
	return (200);
}

int	Client::clientSocketFd( void ) const { return (socket_fd); }

int	Client::getInputFd( void ) const {
	if (!content) {
		return (-1);
	}
	return (content->getInputFd());
}

int	Client::getOutputFd( void ) const {
	if (!content) {
		return (-1);
	}
	return (content->getOutputFd());
}

const sockaddr_storage_t&	Client::getAddr( const LimitExcept& ) const {
	return (client_addr);
}

Http::http_method	Client::getReqMethod( const LimitExcept& ) const {
	return (requests.front().getMethod());
}

std::string	Client::getAddr( void ) const {
	char ip_str[INET6_ADDRSTRLEN];
	if (client_addr.ss_family == AF_INET) {
		struct sockaddr_in* addr_in = (struct sockaddr_in*)&client_addr;
		inet_ntop(AF_INET, &addr_in->sin_addr, ip_str, sizeof(ip_str));
	}
	else if (client_addr.ss_family == AF_INET6) {
		struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)&client_addr;
		inet_ntop(AF_INET6, &addr_in6->sin6_addr, ip_str, sizeof(ip_str));
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

const std::string&	Client::getContentName( void ) const { return(content_name); }

const std::string&	Client::getCurrentUri( void ) const { return (requests.front().getUri()); }

const std::string&	Client::getCurrentUrl( void ) const { return(requests.front().getUrl()); }

const std::string&	Client::getCurrentPath( void ) const { return(requests.front().getPath()); }

const std::string&	Client::getCurrentQuery( void ) const { return(requests.front().getQuery()); }

std::string	Client::getContentType( void ) const { return (requests.front().getContentType()); }

size_t	Client::getContentLength( void ) const { return(requests.front().getContentLength()); }

const std::string&	Client::getHost( void ) const { return(requests.front().getField("host")); }

std::string	Client::getReqMethod( void ) const {
	const char *s[] = { "GET", "POST", "PUT", "DELETE" };
	return (std::string(s[static_cast<int>(requests.front().getMethod())]));
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

