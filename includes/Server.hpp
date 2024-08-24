/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:29:43 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/23 18:47:27 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# include "ServerBlock.hpp"
# include "Client.hpp"

class Server
{
	typedef std::vector<std::pair<int, std::vector<ServerBlock>::iterator> > vector_pair;
	typedef std::vector<ServerBlock>::iterator server_block_iter;
	public:
		Server( void );
		Server( const Server &src );
		~Server( void );

		Server&	operator=( const Server &src );

		void	startServerLoop( int *signal );
		void	startServerLoop( void );

		void		addServerBlock( ServerBlock &ref );
		pollfd_t*	getSocketfds( void );

		static void	setNonBlockFd( int fd );
		static int	setListeningSocket( const sockaddr_in_t &addr, int socket_type, int socket_protocol );

		//getters
		socklen_t	getSocklen( void ) const;
		int			getSocketType( void ) const;
		int			getSocketProtocol( void ) const;
		int			getBacklogLimit( void ) const;
		int			getRecvFlag( void ) const;
		int			getSendFlag( void ) const;
		int			getTimeout( void ) const;
		int			getBufferLimit( void ) const;
		nfds_t		getServerNo( void ) const;
		nfds_t		getServerLimit( void ) const;
		nfds_t		getFdCounter( void ) const;

		//display for ostream
		std::ostream&	displaySocketFds( std::ostream &o ) const;
		std::ostream&	displayServerInfo( std::ostream &o ) const;
		std::ostream&	displayClientInfo( std::ostream &o ) const;

	private:
		typedef std::list<Client>::iterator client_ptr;
		static const socklen_t			socklen = sizeof(sockaddr_in_t);
		static const int				socket_type = SOCK_STREAM;
		static const int				socket_protocol = 0;
		static const int				backlog_limit = 10;
		static const int				recv_flag = 0;
		static const int				send_flag = 0;
		static const int				timeout = (3 * 60 * 1000);
		static const int				buffer_limit = 1024;
		nfds_t							server_no;
		nfds_t							server_limit;
		nfds_t							fd_counter;
		nfds_t							buffer_counter;
		nfds_t							poll_tracker;
		std::vector<pollfd_t>			socket_fds; // load all servers then only add clients (assume all fd on the same vector)
		std::vector<pollfd_t>			buffer_new_fd; // store new fds
		// std::vector<server_block_iter>	server_mapping; // server_index to serverblock_index
		std::vector<std::pair<int, std::vector<ServerBlock>::iterator > > server_mapping;
		std::map<int, client_ptr>		client_mapping; // client fd to client index
		std::vector<ServerBlock>		server_info; // no
		std::list<Client>				client_info; // no

		void	setupServer( void );
		void	setupSocketfds( void );
		void	loopServer( void );
		void	resetFds( void );
		void	getNewConnection( int fd, server_block_iter &it );
		void	closeConnection( size_t index );
		bool	receiveRequest( Client &client );
		bool	sentReponse( Client &client );
		void	fetchData( Client &client );

		uint32_t	findAvaliableSlot( void ) const;
		Client&		getClient( int client_fd );
};

std::ostream&	operator<<( std::ostream &o, std::ostream &o_ );
std::ostream&	operator<<( std::ostream &o, const pollfd_t &ref );
std::ostream&	operator<<( std::ostream &o, const Server& ref );

#endif