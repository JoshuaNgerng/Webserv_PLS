/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:19:05 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/07 14:23:20 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"

ServerSocket::ServerSocket( void ) : domain(AF_INET), service(SOCK_STREAM), protocol(0), s_addr(htonl(INADDR_ANY)) { }

ServerSocket::ServerSocket( const char *ip_add ) :
	domain(AF_INET), service(SOCK_STREAM), protocol(0), s_addr(inet_addr(ip_add)) { }

ServerSocket::ServerSocket( uint64_t ip_add ) :
	domain(AF_INET), service(SOCK_STREAM), protocol(0), s_addr(htonl(ip_add)) { }

ServerSocket::ServerSocket( uint16_t domain_, uint16_t service_, uint16_t protocol_ ) : 
	domain(domain_), service(service_), protocol(protocol_), s_addr(htonl(INADDR_ANY)) { }

ServerSocket::ServerSocket( uint16_t domain_, uint16_t service_, uint16_t protocol_, const char *ip_add ) :
	domain(domain_), service(service_), protocol(protocol_), s_addr(inet_addr(ip_add)) { }

ServerSocket::ServerSocket( uint16_t domain_, uint16_t service_, uint16_t protocol_, uint64_t ip_add ) :
	domain(domain_), service(service_), protocol(protocol_), s_addr(htonl(ip_add)) { }

ServerSocket::ServerSocket( const ServerSocket &src ) :
	domain(src.domain), service(src.service), protocol(src.protocol), s_addr(src.s_addr), addr(src.addr) { }

ServerSocket::~ServerSocket( void ) { }

void	ServerSocket::setPort( int port )
{
	addr = sockaddrInit(port);
}

ServerSocket&	ServerSocket::operator=( const ServerSocket &src )
{
	return (*this);
}

sockaddr_t	ServerSocket::sockaddrInit( int port )
{
	sockaddr_t add = {0};
	add.sin_family = domain;
	add.sin_addr.s_addr = s_addr;
	add.sin_port = htons(port);
	return (add);
}
