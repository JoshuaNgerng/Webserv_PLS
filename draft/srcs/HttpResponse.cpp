/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 06:32:43 by joshua            #+#    #+#             */
/*   Updated: 2024/10/22 23:31:46 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "Server.hpp"

HttpResponse::HttpResponse( void ) { }

HttpResponse::HttpResponse( const HttpResponse &src ) : Http(src) { *this = src; }

HttpResponse::HttpResponse( bool proxy_ ) : proxy(proxy_) { }

HttpResponse::~HttpResponse( void ) { }

HttpResponse&	HttpResponse::operator=( const HttpResponse &src ) {
	if (this == &src) {
		return (*this);
	}
	status = src.status;
	header = src.header;
	body = src.body;
	proxy = src.proxy;
	final = src.final;
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

void	HttpResponse::setHeader( int status, const std::string &str ) {
	std::string	buffer;
	if (!makeReponseHeader(buffer, status)) {
		return ;
	}
	header += "HTTP/1.1 ";
	header += buffer + "\r\n";
	addField("Server", Server::server_name);
	addField("Date", "");
	if (status > 299 && status < 400) {
		addField("Location", str);
	}
	header += "\r\n";
}

void	HttpResponse::setContent( const std::string &type, uint64_t len ) {
	size_t	insert_pos = header.find_last_of("\r\n\r\n");
	if (insert_pos == std::string::npos) {
		return ;
	}
	std::string	buffer, buffer2;
	addField(buffer, "Content-Type", type);
	header.insert(insert_pos, buffer);
	insert_pos += buffer.length();
	buffer2 = to_String(len);
	addField(buffer, "Content-Length", buffer2);
	header.insert(insert_pos, buffer);
}

void	HttpResponse::addBody( const std::string &str ) { body += str; }

void	HttpResponse::finishResponseMsg( void ) {
	final = header + body;
}

void	HttpResponse::reset( void ) {
	status = 0;
	header.clear();
	body.clear();
	proxy = false;
	final.clear();
}

size_t	HttpResponse::getBodyLength( void ) const { return (body.length()); }

size_t	HttpResponse::getTotalLength( void ) const { return (final.length()); }

const char*	HttpResponse::getPtrPos( size_t no_bytes_send ) const {
	return (final.c_str() + no_bytes_send);
}