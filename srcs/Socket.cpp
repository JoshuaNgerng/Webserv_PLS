/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:44:50 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/14 15:11:40 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket( void ) { checkLen(); }

Socket::Socket( uint8_t domain ) { addr.sin_family = domain; checkLen(); }

Socket::Socket( const Socket &src ) : port(src.port), addr(src.addr) { }

Socket::~Socket( void ) { }

Socket&	Socket::operator=( const Socket &src ){
	port = src.port;
	addr = src.addr;
	return (*this);
}

void	Socket::checkLen( void ){
	#ifdef __APPLE__
	addr.sin_len = sizeof(sockaddr_t);
	#endif
}

bool	Socket::operator==( const Socket &src ){
	if (port != src.port)
		return (false);
	if (addr.sin_family != src.addr.sin_family)
		return (false);
	if (addr.sin_port != src.addr.sin_port)
		return (false);
	if (addr.sin_addr.s_addr != src.addr.sin_addr.s_addr)
		return (false);
	return (true);
}

sockaddr_t&	Socket::changeAddress( void ) { return (addr); }

const sockaddr_t&	Socket::refAddress( void ) const { return (addr); }
