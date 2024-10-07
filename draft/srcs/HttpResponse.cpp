/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 06:32:43 by joshua            #+#    #+#             */
/*   Updated: 2024/10/07 16:37:27 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "Server.hpp"

HttpResponse::HttpResponse( void ) { }

HttpResponse::HttpResponse( bool proxy_ ) : proxy(proxy_) { }

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
