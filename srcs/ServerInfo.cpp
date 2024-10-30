/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:09:04 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/29 14:52:05 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerInfo.hpp"
#include "Client.hpp"

ServerInfo::ServerInfo( void ) : 
InfoBlock(),
listen_sockets(),
server_name(),
client_header_buffer_size(0),
client_header_timeout(0),
merge_slash(false),
ignore_invalid_header(false),
location()
{}


ServerInfo::ServerInfo( const ServerInfo &src ) : InfoBlock(src) {
	*this = src;
}

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

bool	ServerInfo::isMergeSlash( void ) const { return (merge_slash); }

void	ServerInfo::matchUri( Client &client ) const {
	typedef std::vector<Location>::const_iterator iter;
	const std::string	&uri = client.getCurrentUri();
	iter				ptr = location.end();
	size_t				len = 0;
	for (iter it = location.begin(); it != location.end(); it ++) {
		size_t check = it->getLocationPath().length();
		if (!uri.compare(0, check, it->getLocationPath())) {
			if (check > len) {
				len = check;
				ptr = it;
			}
		}
	}
	client << ptr;
	bool	b = (autoindex == on) ? true : false;
	if (ptr != location.end()) {
		ptr->matchUri(client, b);
	}
	InfoBlock::matchUri(client, b);
}

std::vector<ListenSocket>::const_iterator	ServerInfo::listenBegin( void ) const {
	return (listen_sockets.begin());
}

std::vector<ListenSocket>::const_iterator	ServerInfo::listenEnd( void ) const {
	return (listen_sockets.end());
}

std::vector<Location>::const_iterator	ServerInfo::getLocEnd( void ) const {
	return (location.end());
}

std::ostream&	operator<<( std::ostream &o, const ServerInfo &ref ) {
	(void)ref;
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
