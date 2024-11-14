/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:09:04 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/15 06:03:43 by jngerng          ###   ########.fr       */
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
location()
{}


ServerInfo::ServerInfo( const ServerInfo &src ) : InfoBlock() {
	*this = src;
}

ServerInfo::~ServerInfo( void ) { }

ServerInfo&	ServerInfo::operator=( const ServerInfo &src ) {
	if (this == &src) {
		return (*this);
	}
	InfoBlock::operator=(src);
	listen_sockets = src.listen_sockets;
	server_name = src.server_name;
	client_header_buffer_size = src.client_header_buffer_size;
	client_header_timeout = src.client_header_timeout;
	merge_slash = src.merge_slash;
	location = src.location;
	return (*this);
}

void	ServerInfo::clearListenAddr( void ) {
	typedef	std::vector<ListenSocket>::iterator	iter;
	for (iter it = listen_sockets.begin(); it != listen_sockets.end(); it ++) {
		it->clearAddr();
	}
}

void	ServerInfo::emptyListenAddr( void ) {
	typedef	std::vector<ListenSocket>::iterator	iter;
	for (iter it = listen_sockets.begin(); it != listen_sockets.end(); it ++) {
		it->emptyAddrPtr();
	}
}

void	ServerInfo::reset( void ) {
	InfoBlock::reset();
	emptyListenAddr();
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

void	ServerInfo::routingClient( Client &client ) const {
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
	if (ptr != location.end()) {
		ptr->routingClient(client);
		return ;
	}
	InfoBlock::routingClient(client);
}

void	ServerInfo::defaultSetting( void ) {
	typedef std::vector<Location>::iterator	iter;
	InfoBlock::defaultSetting();
	for (iter it = location.begin(); it != location.end(); it ++) {
		it->defaultSetting(*this);
	}
	if (!listen_sockets.size()) {
		ListenSocket	buffer;
		listen_sockets.push_back(buffer);
		listen_sockets[0].addAddress("::", "80");
	}
	if (!server_name.size()) {
		server_name.push_back("");
	}
	if (!client_header_buffer_size) {
		client_header_buffer_size = 1000;
	}
	if (!client_header_timeout) {
		client_header_timeout = 60;
	}
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

size_t	ServerInfo::getHeaderSizeLimit( void ) const { return (client_header_buffer_size); }

size_t	ServerInfo::getHeaderTimeout( void ) const { return (client_header_timeout); }

void	ServerInfo::displayAddrPort( std::ostream &o ) const {
	typedef std::vector<ListenSocket>::const_iterator	sock_iter;
	for (sock_iter it = listen_sockets.begin(); it != listen_sockets.end(); it ++) {
		o << *it << " ";
	}
}

std::ostream&	operator<<( std::ostream &o, const ServerInfo &ref ) {
	(void)ref;
	o << "Listening on: ";
	ref.displayAddrPort(o);
	o << '\n';
	return (o);
}
