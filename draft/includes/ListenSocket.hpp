/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:02:23 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/20 14:41:12 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENSOCKET_HPP
# define LISTENSOCKET_HPP
# include "Const.hpp"

class ListenSocket {
	public:
		ListenSocket( void );
		ListenSocket( const ListenSocket &src );
		~ListenSocket( void );

		ListenSocket&	operator=( const ListenSocket &src );

		bool	addAddress( const std::string &addr, const std::string &port );
		int		addListenPollFd( std::vector<pollfd_t> &listen ) const;
		void	setDefaultServer( void );
		void	setBackLog( uint64_t num );
		void	setRcvBuf( uint64_t num );
		void	setSndBuf( uint64_t num );
		void	setIpv6( void );
		void	setReusePort( void );
		void	setKeepalive( long idle, long intvl, long cnt );
		void	setKeepalive( void );
		void	clear( void );
		void	reset( void );

		class Iterator {
			public:
				Iterator( void );
				Iterator( const Iterator &src );
				Iterator( addrinfo_t *start );
				~Iterator( void );

				Iterator&	operator=( const Iterator &src );
				addrinfo_t&	operator*( void );
				addrinfo_t*	operator->( void ) const;
				Iterator&	operator++( void ); 
				Iterator	operator++( int );
				Iterator&	operator+( size_t n );
				bool		operator!=( const Iterator& other ) const;
				bool		operator==( const Iterator& other ) const;

			private:
				addrinfo_t	*ptr;
		};

		Iterator	begin( void ) const;
		Iterator	end( void ) const;
		uint32_t	length( void ) const;
		int			getStatus( void ) const;
		int			addListenFd( const Iterator &it ) const;

	private:
		addrinfo_t	*addr_info_head;
		addrinfo_t	**addr_info_tail;
		bool		default_server;
		uint16_t	backlog;
		uint64_t	rcvbuf_size;
		uint64_t	sndbuf_size;
		bool		ipv6only;
		bool		reuseport;
		bool		keepalive;
		long		keepidle;
		long		keepintvl;
		long		keepcnt;
		uint32_t	len;
		int			status;

		bool	socketSetup( int fd ) const;
};

/*
listen address[:port] [default_server] [ssl] [http2 | quic] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
listen port [default_server] [ssl] [http2 | quic] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
listen unix:path [default_server] [ssl] [http2 | quic] [proxy_protocol] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
*/

std::ostream&	operator<<( std::ostream &o, const ListenSocket& ref );

#endif
