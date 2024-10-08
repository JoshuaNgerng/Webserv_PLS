/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:29:43 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/09 15:09:30 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# include "ServerBlock.hpp"
# include "Client.hpp"

class Server
{
	typedef std::vector<ServerBlock>::iterator serverblock_ptr;
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
		static const int				fcntl_flag = (O_NONBLOCK );
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
		std::vector<serverblock_ptr>	server_mapping; // server_index to serverblock_index
		std::map<int, client_ptr>		client_mapping; // client fd to client index
		std::vector<ServerBlock>		server_info;
		std::list<Client>				client_info;

		void	setupServer( void );
		void	setupSocketfds( void );
		void	loopServer( void );
		void	resetFds( void );
		bool	checkBufferfds( void ) const;
		void	addBufferfds( int fd );
		void	handleServer( size_t index );
		void	handleClient( size_t index );
		void	handleClientRecv( pollfd_t &pollfd, client_ptr &ptr, size_t index );
		void	handleClientSent( pollfd_t &pollfd, client_ptr &ptr, size_t index );
		
		void	getNewConnection( int fd, serverblock_ptr &it );
		void	clearClient( size_t index );
		void	resetPollFd( pollfd_t &pollfd );
		void	fetchClientData( client_ptr &ptr );

		// bool	receiveData( int fd , std::string &output ) const;
		// bool	receiveRequest( Client &client );
		// bool	sentReponseToClient( Client &client );
		// void	fetchData( Client &client );
		// bool	receiveFromClient( Client &ptr, int fd );

		uint32_t	findAvaliableSlot( void ) const;
		Client&		getClient( int client_fd );
		// bool		clientReponseStatus( Client &ptr ) const;
		// bool		fetchReponseData( Client &client ) ;
		// void		getReponseDataFd( Client &client );

};

std::ostream&	operator<<( std::ostream &o, std::ostream &o_ );
std::ostream&	operator<<( std::ostream &o, const pollfd_t &ref );
std::ostream&	operator<<( std::ostream &o, const Server& ref );

#endif
