/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:09:04 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/26 04:58:44 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerInfo.hpp"

// ServerInfo::ServerInfo( void ) : listen(), server_name(),
// 	root(), client_max_body_size(ULONG_MAX), index(),
// 	autoindex(false), error_page() { }

// ServerInfo::ServerInfo( const ServerInfo &src ) : listen(src.listen),
// 	server_name(src.server_name), root(src.root),
// 	client_max_body_size(src.client_max_body_size), index(src.index),
// 	autoindex(src.autoindex), error_page(src.error_page), location(src.location)
// 	{ }

ServerInfo::~ServerInfo( void ) { }

ServerInfo&	ServerInfo::operator=( const ServerInfo &src ) { (void)src; return (*this); }

void	ServerInfo::reset( void ) {
	InfoBlock::reset();
	listen_sockets.clear();
	server_name.clear();
	location.clear();
}

void	ServerInfo::addLocation( const Location &add ) { location.push_back(add); }

void	ServerInfo::addListen( const ListenSocket &add ) { listen_sockets.push_back(add); }

void	ServerInfo::addServerName( const std::string &add ) { server_name.push_back(add); }

void	ServerInfo::setClientHeaderBufferSize( uint64_t size ) { client_header_buffer_size = size;}

void	ServerInfo::setClientHeaderTimeout( uint64_t time ) { client_header_timeout = time; }

void	ServerInfo::setMergeSlash( bool opt ) { merge_slash = opt; }

std::ostream&	operator<<( std::ostream &o, const ServerInfo &ref ) {
	// typedef std::vector<ListenSocket>::const_iterator		sock_iter;
	// typedef std::map<uint16_t, std::string>::const_iterator	error_iter;
	o << "Listening on: ";
	// for (sock_iter it = ref.listen.begin(); it != ref.listen.end(); it ++) {
	// 	o << *it << ' ';
	// }
	// o << '\n';
	// o << "Server name: " << ref.server_name << '\n';
	// o << "Root directory: " << ref.root << '\n';
	// o << "Client max body size: " << ref.client_max_body_size << '\n';
	// o << "Index: " << ref.index << '\n';
	// o << "Error pages: ";
	// if (ref.error_page.begin() == ref.error_page.end())
	// 	return (o);
	// for (error_iter it = ref.error_page.begin(); it != ref.error_page.end(); it ++) {
	// 	o << it->first << " = " << it->second << ' ';
	// }
	o << '\n';
	return (o);
}

// std::string	ServerInfo::testHTML( void ) {
// 	std::string	msg = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
// 	std::string	html = "<html><h1>Hello world</h1></html?";
// 	msg.append(to_String(html.size()));
// 	msg.append("\n\n");
// 	msg.append(html);
// 	return (msg);
// }
