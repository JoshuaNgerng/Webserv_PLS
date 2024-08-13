/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tcp.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 17:21:08 by jngerng           #+#    #+#             */
/*   Updated: 2024/06/13 13:58:19 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include "Colour.hpp"

using std::string;

// typedef struct address
// {
// 	;
// }	address;

// typedef struct sockaddr_in {
// 	int16_t		sin_family;		// e.g. AF_INET
// 	uint16_t	sin_port;		// e.g. htons(8080)
// 	uint64_t	sin_addr;		// (they use struct in_addr with a single unsigned long member, below)
// 	char		sin_zero[8];	// zero this if you want to
// }	sockaddr_t;

typedef struct sockaddr_in sa;

class Tcp
{
	private:
		static const int	max_len_ip = 25;

		const int	domain;
		const int	service;
		const int	protocol;

		char	buffer[max_len_ip + 1];

	protected:
		const string	ip_addr;
		const int		m_port;
		int				socket_fd;
		sa				address;

	public:
		Tcp( void );
		Tcp( string ip_addr_, int port );
		Tcp( string ip_addr_, int port, int domain, int service, int protocol);
		~Tcp( void );

		void	startServer( void );
		string	IPAddressRep( sa ip );

		virtual void	establishConnection( void ) = 0;

	class	TcpError : public std::exception
	{
		private:
			string	msg;
			void	getMsg( const char * msg_ );
		public:
			TcpError( const char * msg_ );
			~TcpError( void ) throw();
			const char* what() const throw() { return (msg.c_str()); }
	}	;
}	;
