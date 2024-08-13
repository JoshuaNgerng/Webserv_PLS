/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tcp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 17:21:46 by jngerng           #+#    #+#             */
/*   Updated: 2024/06/14 15:21:50 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tcp.hpp"

Tcp::Tcp( void ) : domain(AF_INET), service(SOCK_STREAM), protocol(0),
					ip_addr("127.0.0.1"), m_port(8080) { }

Tcp::Tcp( string ip_addr_, int port ) : domain(AF_INET), service(SOCK_STREAM), protocol(0),
										ip_addr(ip_addr_), m_port(port) { }

Tcp::Tcp( string ip_addr_, int port, int domain, int service, int protocol ) : 
	domain(domain), service(service), protocol(protocol),
	ip_addr(ip_addr_), m_port(port) { }

Tcp::~Tcp( void ) { close(socket_fd); }

void	Tcp::startServer( void )
{
	if ((socket_fd = socket(domain, service, protocol)) < 0)
		throw TcpError("Cannot establish socket");
	address.sin_family = domain;
	address.sin_addr.s_addr = inet_addr(ip_addr.c_str());
	address.sin_port = htons(m_port);
}

string	Tcp::IPAddressRep( sa ip )
{
	inet_ntop(domain, &ip, buffer, max_len_ip);
	return (string(buffer, max_len_ip));
}

Tcp::TcpError::TcpError( const char * msg_ ) : msg(BOLDRED) { getMsg(msg_); }

Tcp::TcpError::~TcpError( void ) throw() { }

void	Tcp::TcpError::getMsg( const char * msg_ )
{
	const char	*ptr;

	ptr = std::strerror(errno);
	msg += msg_;
	if (errno)
	{
		msg += ": ";
		msg += ptr;
	}
	msg += RESET;
}
