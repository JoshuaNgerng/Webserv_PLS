/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:02:23 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/14 01:11:00 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "Const.hpp"

class Socket {
	public:
		Socket( void );
		Socket( uint8_t	domain );
		Socket( const Socket &src );
		~Socket( void );

		Socket&		operator=( const Socket &src );
		bool		operator==( const Socket &src );

		sockaddr_in_t&			changeAddress( void );
		const sockaddr_in_t&	refAddress( void ) const;

	private:
		void	checkLen( void );

		std::string		unix_path;
		sockaddr_in_t	addr;
		bool			default_server;
		uint16_t		backlog;
		uint64_t		rcvbuf_size;
		uint64_t		sndbuf_size;
};

/*
listen address[:port] [default_server] [ssl] [http2 | quic] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
listen port [default_server] [ssl] [http2 | quic] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
listen unix:path [default_server] [ssl] [http2 | quic] [proxy_protocol] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
*/

std::ostream&	operator<<( std::ostream &o, const Socket& ref );

#endif
