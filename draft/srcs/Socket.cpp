/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:44:50 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/17 10:20:06 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket( void ) { checkLen(); }

Socket::Socket( uint8_t domain ) { addr.sin_family = domain; checkLen(); }

Socket::Socket( const Socket &src ) : addr(src.addr) { }

Socket::~Socket( void ) { }

Socket&	Socket::operator=( const Socket &src ){ addr = src.addr; return (*this); }

void	Socket::checkLen( void ){
	#ifdef __APPLE__
	addr.sin_len = sizeof(sockaddr_t);
	#endif
}

bool	Socket::operator==( const Socket &src ){
	if (addr.sin_family != src.addr.sin_family ||
		addr.sin_port != src.addr.sin_port || 
		addr.sin_addr.s_addr != src.addr.sin_addr.s_addr) {
		return (false);
	}
	return (true);
}

sockaddr_in_t&	Socket::changeAddress( void ) { return (addr); }

const sockaddr_in_t&	Socket::refAddress( void ) const { return (addr); }

std::ostream&	operator<<( std::ostream &o, const Socket& ref ) {
	const sockaddr_in_t	&addr = ref.refAddress();
	char	buffer[INET_ADDRSTRLEN];
	o << inet_ntop(addr.sin_family, &addr.sin_addr, buffer, INET_ADDRSTRLEN) <<
		':' << ntohs(addr.sin_port);
	return (o);
}
