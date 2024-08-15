/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:29:43 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/16 01:43:13 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# include "ServerBlock.hpp"
# include "Client.hpp"

class Server
{
	public:
		Server( void );
		Server( const Server &src );
		~Server( void );

		Server&	operator=( const Server &src );

		void	startServerLoop( int *signal );
		void	startServerLoop( void );

		std::vector<ServerBlock>&	parseServerInfo( void );
		pollfd_t*					getSocketfds( void );

	private:
		typedef std::list<Client>::iterator client_ptr;
		static const socklen_t		socklen = sizeof(sockaddr_in_t);
		static const int			socket_type = SOCK_STREAM;
		static const int			socket_protocol = 0;
		static const int			backlog_limit = 10;
		static const int			recv_flag = 0;
		static const int			send_flag = 0;
		static const int			timeout = 1000;
		static const int			buffer_limit = 1024;
		uint32_t					server_no;
		nfds_t						server_limit;
		uint32_t					server_index;
		std::vector<pollfd_t>		socket_fds; // load all servers then only add clients (assume all fd on the same vector)
		std::vector<size_t>			server_mapping; // server_index to serverblock_index
		std::map<int, client_ptr>	client_mapping; // client fd to client index
		std::vector<ServerBlock>	server_info;
		std::list<Client>			client_info;

		void	setupServer( void );
		void	setupSocketfds( void );
		void	loopServer( void );
		void	getNewConnection( size_t index );
		void	closeConnection( size_t index );
		bool	receiveRequest( Client &client );
		bool	sentReponse( Client &client );
		void	fetchData( Client &client );

		uint32_t	findAvaliableSlot( void ) const;
		Client&		getClient( int client_fd );
};

#endif
