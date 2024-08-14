/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:29:43 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/14 17:42:41 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# include "ServerBlock.hpp"

class Server
{
	public:
		Server( void );
		Server( const Server &src );
		~Server( void );

		Server&	operator=( const Server &src );

		void	startServerLoop( void );

		std::vector<ServerBlock>&	parseServerInfo( void );
		pollfd_t*					getSocketfds( void );

	private:
		static const socklen_t		socklen = sizeof(sockaddr);
		static const int			socket_type = SOCK_STREAM;
		static const int			socket_protocol = 0;
		static const int			backlog_limit = 10;
		static const int			timeout = 1000;
		uint32_t					server_no;
		nfds_t						server_limit;
		uint32_t					server_index;
		std::vector<pollfd_t>		socket_fds; // load all servers then only add clients (assume all fd on the same vector)
		std::vector<int, int>		server_mapping; // server_index to serverblock_index
		std::map<pollfd_t, int>		client_mapping; // client fd to server_index
		std::vector<ServerBlock>	server_info;

		void	setupServer( void );
		void	setupSocketfds( void );
		void	loopServer( void );
		void	getNewConnection( int server_fd );
		void	receiveRequest( int client_fd );
		void	sentReponse( int client_fd );

		uint32_t	findAvaliableSlot( void ) const;
};

#endif
