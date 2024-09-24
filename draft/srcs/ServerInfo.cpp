/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:09:04 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/24 15:44:09 by jngerng          ###   ########.fr       */
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

/**
 * @brief	iter through the token till `;' special char is found
 * 			there a few different possible entry
 * 			only port number (assume any ip add)
 * 			[::]:7000 (special for any ip add) , bind to a port number
 * 			127.0.0.1:8080, a specfic ip add , bind to a port number
 * 			it will add the map, port as key and address info as value
 * 
 * @attention	assume our subject only need us to handle ipv4 (ipv6 is a pain lulz)
 * 				if need to add ipv6 then need to edit this sht lulz
 * 				assume https dont exist for now
 * 
 * @param	stream : the stringstream that from Parse class
 * 
 * @throws	ParsingError class , options: invalid_ip_add, repeated_port
*/
void	ServerInfo::addListen( const ListenSocket &add ) { listen_sockets.push_back(add); }

void	ServerInfo::reset( void ) {
	InfoBlock::reset();
	listen_sockets.clear();
	server_name.clear();
	location.clear();
}

void	ServerInfo::addListen( const ListenSocket &add ) { listen_sockets.push_back(add); }

void	ServerInfo::addLocation( const Location &add ) { location.push_back(add); }

void	ServerInfo::addServerName( const std::string &add ) { server_name.push_back(add); }

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
