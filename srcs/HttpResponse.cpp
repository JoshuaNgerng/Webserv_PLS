/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 06:32:43 by joshua            #+#    #+#             */
/*   Updated: 2024/11/05 18:13:58 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "Server.hpp"

HttpResponse::HttpResponse( void ) :
status(0),
proxy(false)
{ }

HttpResponse::HttpResponse( bool proxy_ ) :
status(0),
proxy(proxy_)
{ }

HttpResponse::HttpResponse( const HttpResponse &src ) : Http(src) { *this = src; }

HttpResponse::~HttpResponse( void ) { }

HttpResponse&	HttpResponse::operator=( const HttpResponse &src ) {
	if (this == &src) {
		return (*this);
	}
	status = src.status;
	proxy = src.proxy;
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

void	HttpResponse::addBody( const std::string &str ) { body += str; }

void	HttpResponse::addBody( const char *str, size_t bytes ) { body.append(str, bytes); }

void	HttpResponse::setHeader( int status, const std::string &str ) {
	std::string	buffer;
	if (!makeReponseHeader(buffer, status)) {
		return ;
	}
	header += "HTTP/1.1 ";
	header += buffer + "\r\n";
	addField("Server", Server::server_name);
	std::time_t current_time = std::time(NULL);
	struct tm* time_info = std::gmtime(&current_time);
	char date_buffer[100];
	if (strftime(date_buffer, sizeof(date_buffer), "%a, %d %b %Y %H:%M:%S GMT", time_info)) {
		std::string date_string(date_buffer);
		addField("Date", date_string);
	}

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

bool	HttpResponse::isReady( void ) const { return (ready); }

std::ostream&	operator<<( std::ostream &o, const HttpResponse &res ) {
	o << "Http Response " << ((!res.isReady()) ? "not " : " " ) << "ready\n";
	o << res.getPtr2Http(0) << '\n';
	return (o);
}
