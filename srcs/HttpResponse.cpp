/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 06:32:43 by joshua            #+#    #+#             */
/*   Updated: 2024/11/14 21:57:54 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "Server.hpp"

HttpResponse::HttpResponse( void ) : status(0) { }

HttpResponse::HttpResponse( const HttpResponse &src ) : Http(src) { *this = src; }

HttpResponse::~HttpResponse( void ) { }

HttpResponse&	HttpResponse::operator=( const HttpResponse &src ) {
	if (this == &src) {
		return (*this);
	}
	Http::operator=(src);
	status = src.status;
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

void	HttpResponse::setContent( uint64_t len, const std::string &type ) {
	size_t	insert_pos = header.length();
	if (insert_pos > 2) {
		insert_pos -= 2;
	}
	if (type.length() > 0) {

	}
	std::string	buffer;
	if (type.length() > 0) {
		addField(buffer, "Content-Type", type);
		content_type = type;
		header.insert(insert_pos, buffer);
		insert_pos += buffer.length();
		buffer.clear();
	}
	if (len) {
		addField(buffer, "Content-Length", to_String(len));
		content_length = len;
		header.insert(insert_pos, buffer);
	}
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
	Http::reset();
	status = 0;
	ready = false;
}

bool	HttpResponse::isReady( void ) const { return (ready); }

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

bool	HttpResponse::generateHeaderHelper( const std::string &line ) {
	std::string	field, val;
	size_t 		pos = line.find(':');
	if (pos == std::string::npos) {
		return (false) ;
	}
	field = line.substr(0, pos);
	val = line.substr(pos + 1);
	if (!validateField(field) || !validateValue(val) ||
		line[line.length() - 1] != '\r') {
		return (false) ;
	}
	if (field == "content-length") {
		content_length = std::atoll(val.c_str());
	} else if (field == "content-type") {
		content_type = val;
	}
	header += line + '\n';
	return (true);
}

bool	HttpResponse::generateHeader( const std::string &buffer ) {
	std::istringstream	ss(buffer);
	std::string			line; 
	std::getline(ss, line);
	if (validateHttpStart(line)) {
		header += line + '\n';
	} else {
		addHeaderStart(200);
		generateHeaderHelper(line);
	}
	addHeader();
	while (std::getline(ss, line)) {
		generateHeaderHelper(line);
	}
	header += "\r\n";
	return (true);	
}

bool	HttpResponse::processCgiDataHeader( void ) {
	size_t	pos = combine.find("\r\n\r\n");
	if (pos == std::string::npos) {
		return (false);
	}
	pos += 4;
	std::string	buffer = combine.substr(0, pos);
	combine.erase(0, pos);
	header_ready = true;
	generateHeader(buffer);
	return (true);
}

bool	HttpResponse::processCgiData( void ) {
	if (!header_ready) {
		size_t pos = combine.find("\r\n\r\n");
		if (pos == std::string::npos) {
			setHeader(200);
			setContent(combine.length(), "application/octet-stream");
		} else {
			std::string	buffer = combine.substr(0, pos + 4);
			combine.erase(0, pos + 4);
			generateHeader(buffer);
		}
	}
	if (!content_length) {
		setContent(combine.length());
	}
	if (!content_type.length()) {
		setContent(0, "application/octet-stream");
	}
	if (combine.length() > content_length) {
		combine.erase(content_length);
	}
	combine = header + combine;
	ready = true;
	bytes_sent = 0;
	return (true);
}

std::ostream&	operator<<( std::ostream &o, const HttpResponse &res ) {
	o << "Http Response " << ((!res.isReady()) ? "not " : " " ) << "ready\n";
	o << res.getPtr2Http() << '\n';
	return (o);
}
