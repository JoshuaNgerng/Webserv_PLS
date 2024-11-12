/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 06:32:43 by joshua            #+#    #+#             */
/*   Updated: 2024/11/12 15:48:44 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "Server.hpp"

HttpResponse::HttpResponse( void ) :
status(0),
proxy(false),
bytes_sent(0)
{ }

HttpResponse::HttpResponse( bool proxy_ ) :
status(0),
proxy(proxy_),
bytes_sent(0)
{ }

HttpResponse::HttpResponse( const HttpResponse &src ) : Http(src) { *this = src; }

HttpResponse::~HttpResponse( void ) { }

HttpResponse&	HttpResponse::operator=( const HttpResponse &src ) {
	if (this == &src) {
		return (*this);
	}
	status = src.status;
	proxy = src.proxy;
	bytes_sent = src.bytes_sent;
	return (*this);
}

void	HttpResponse::addField( std::string &str, const char *name, const std::string &val ) const {
	str += name;
	str += ": ";
	str += val;
	str += "\r\n";
}

void	HttpResponse::addField( const char *name, const std::string &val ) {
	header += name;
	header += ": ";
	header += val;
	header += "\r\n";
}

void	HttpResponse::addField( const char *name, const char *val ) {
	header += name;
	header += ": ";
	header += val;
	header += "\r\n";
}

void	HttpResponse::addField( const std::string &name, const std::string &val ) {
	header += name;
	header += ": ";
	header += val;
	header += "\r\n";
}

void	HttpResponse::addFin( const std::string &str ) { combine += str; }

void	HttpResponse::addFin( const char *str, size_t bytes ) { combine.append(str, bytes); }

void	HttpResponse::addBody( const std::string &str ) { body += str; }

void	HttpResponse::addBody( const char *str, size_t bytes ) { body.append(str, bytes); }

void	HttpResponse::addHeaderStart( int status ) {
	std::string	buffer;
	if (!makeReponseHeader(buffer, status)) {
		return ;
	}
	header += "HTTP/1.1 ";
	header += buffer + "\r\n";
}

void	HttpResponse::addHeader( void ) {
	addField("Server", Server::server_name);
	std::time_t current_time = std::time(NULL);
	struct tm* time_info = std::gmtime(&current_time);
	char date_buffer[100];
	if (strftime(date_buffer, sizeof(date_buffer), "%a, %d %b %Y %H:%M:%S GMT", time_info)) {
		std::string date_string(date_buffer);
		addField("Date", date_string);
	}
}

void	HttpResponse::addHeader( int status ) {
	addHeaderStart(status);
	addHeader();
}

void	HttpResponse::setHeader( int status, const std::string &str ) {
	addHeader(status);
	if (status > 299 && status < 400) {
		addField("Location", str);
	}
	header += "\r\n";
}

void	HttpResponse::setContent( const std::string &type, uint64_t len ) {
	size_t	insert_pos = header.length();
	if (insert_pos > 2) {
		insert_pos -= 2;
	}
	// std::cout << "test response insert pos " << insert_pos << '\n';
	std::string	buffer;
	addField(buffer, "Content-Type", type);
	header.insert(insert_pos, buffer);
	insert_pos += buffer.length();
	buffer.clear();
	addField(buffer, "Content-Length", to_String(len));
	header.insert(insert_pos, buffer);
}

void	HttpResponse::setContent( void ) {
	size_t	insert_pos = header.length();
	if (insert_pos > 2) {
		insert_pos -= 2;
	}
	std::string	buffer;
	addField(buffer, "Content-Length", "0");
	header.insert(insert_pos, buffer);
}

void	HttpResponse::reset( void ) {
	status = 0;
	proxy = false;
	ready = false;
}

size_t	HttpResponse::getBodyLength( void ) const { return (body.length()); }

size_t	HttpResponse::getTotalLength( void ) const { return (combine.length()); }

size_t	HttpResponse::getRemainderLength( void ) const {
	if (bytes_sent > combine.length()) { return (0); }
	return (combine.length() - bytes_sent);
}

bool	HttpResponse::isReady( void ) const { return (ready); }

bool	HttpResponse::sendStateTotal( size_t bytes ) {
	bytes_sent += bytes;
	if (bytes_sent >= combine.length()) {
		return (false);
	}
	return (true);
}

bool	HttpResponse::validateHttpStart( const std::string &line ) const {
	std::string buffer;
	size_t i = 0, j = 0;
	for (; i < line.length(); i ++) {
		if (line[i] == ' ')
			break ;
	}
	if (!i) {
		return (false);
	}
	buffer = line.substr(0, i);
	if (buffer != "HTTP/1.1") {
		return (false);
	}
	i ++;
	j = i;
	for (; i < line.length(); i ++) {
		if (line[i] == ' ')
			break ;
	}
	if (j == i) {
		return (false);
	}
	buffer = line.substr(j, i - j);
	int	check = std::atoi(buffer.c_str());
	if (check < 100 || check > 599) {
		return (false);
	}
	if (line[line.length() - 1] != '\r') {
		return (false);
	}
	return (true);
}

bool	HttpResponse::generateHeader( const std::string &buffer ) {
	std::istringstream	ss(buffer);
	std::string			line, field, val;
	bool				content_length = false;
	bool				content_type = false;
	std::getline(ss, line);
	if (!validateHttpStart(line)) {
		addHeader(200);
	} else {
		header += line;
		std::getline(ss, line);
		if (!line.length()) {
			addHeader();
			return (true);
		}
	}
	addHeader();
	while (std::getline(ss, line)) {
		size_t pos = line.find('=');
		if (pos == std::string::npos) {
			continue ;
		}
		field = line.substr(0, pos);
		val = line.substr(pos + 1);
		if (!validateField(field) || !validateField(val) ||
			line[line.length() - 1] != '\r') {
			continue ;
		}
		if (field == "content-length") {
			content_length = true;
		} else if (field == "content-type") {
			content_type = true;
		}
		header += line + '\n';
	}
	if (!content_type) {
		addField("Content-Type", "application/octet-stream");
	}
	if (!content_length) {
		addField("Content-Length", to_String(combine.length()));
	}
	return (true);	
}

bool	HttpResponse::processCgiData( void ) {
	size_t pos = combine.find("\r\n\r\n");
	if (pos == std::string::npos) {
		setHeader(200);
		setContent("application/octet-stream", combine.length());
	} else {
		std::string	buffer = combine.substr(0, pos + 4);
		combine.erase(0, pos + 4);
		generateHeader(buffer);
	}
	combine = header + combine;
	ready = true;
	return (true);
}

std::ostream&	operator<<( std::ostream &o, const HttpResponse &res ) {
	o << "Http Response " << ((!res.isReady()) ? "not " : " " ) << "ready\n";
	o << res.getPtr2Http(0) << '\n';
	return (o);
}
