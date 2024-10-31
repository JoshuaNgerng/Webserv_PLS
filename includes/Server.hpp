/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:29:43 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/30 17:13:15 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# include "ServerInfo.hpp"
# include "Client.hpp"

class Server
{
	typedef std::vector<ServerInfo>::const_iterator		serverinfo_ptr;
	typedef std::vector<ListenSocket>::const_iterator	listen_ptr;
	typedef ListenSocket::Iterator						addrinfo_ptr;
	public:
		static const char *server_name;
		Server( void );
		Server( const Server &src );
		Server&	operator=( const Server &src );
		~Server( void );

		void		startServerLoop( void );
		static void	signalHandler( int signal );

		void		clearListenAddr( void );
		void		addServerInfo( ServerInfo &ref );
		pollfd_t*	getSocketfds( void );

		//getters
		socklen_t	getSocklen( void ) const;
		int			getSocketType( void ) const;
		int			getSocketProtocol( void ) const;
		nfds_t		getServerNo( void ) const;
		nfds_t		getServerLimit( void ) const;
		nfds_t		getFdCounter( void ) const;
		int			getTimeout( void ) const;

		//display for ostream
		std::ostream&	displaySocketFds( std::ostream &o ) const;
		std::ostream&	displayServerInfo( std::ostream &o ) const;
		std::ostream&	displayClientInfo( std::ostream &o ) const;

	private:
		typedef std::list<Client>::iterator client_ptr;
		static const socklen_t	socklen = sizeof(sockaddr_in_t);
		static const int		socket_type = SOCK_STREAM;
		static const int		socket_protocol = 0;
		static const int		fcntl_flag = (O_NONBLOCK );
		static const int		timeout = (3 * 60 * 1000);
		static volatile bool	running;

		nfds_t	server_no;
		nfds_t	server_limit;
		nfds_t	fd_counter;
		nfds_t	buffer_counter;
		nfds_t	poll_tracker;

		std::vector<pollfd_t>		socket_fds;
		std::vector<pollfd_t>		buffer_new_fd;
		std::vector<serverinfo_ptr>	server_mapping; // server_index to ServerInfo_index
		std::vector<addrinfo_ptr>	socketfd_mapping;
		std::map<int, client_ptr>	client_mapping; // client fd to client index
		std::vector<ServerInfo>		server_info;
		std::list<Client>			client_info;

		void	setupSockets( void );
		void	setupSocketsListen( serverinfo_ptr ptr, pollfd_t &buffer );
		int		setupSocketsCheckError( listen_ptr ptr, addrinfo_ptr addr );
		void	loopServer( void );
		void	resetFds( void );
		bool	checkBufferfds( void ) const;
		void	addBufferfds( int fd );
		void	addBufferfds( int fd, int events );
		void	handleServer( size_t index );
		void	handleClient( size_t index );
		void	handleClientRecv( pollfd_t &pollfd, Client &client );
		void	handleClientSent( pollfd_t &pollfd, Client &client );
		void	addClientContentFd( client_ptr client );

		void	error2Client( int fd, client_ptr client );
		void	markAsDelete( pollfd_t &pollfd );
		void	markAsDelete( pollfd_t &pollfd, Client &client );
		void	fetchClientData( int fd );

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

std::ostream&	operator<<( std::ostream &o, const pollfd_t &ref );
std::ostream&	operator<<( std::ostream &o, const Server& ref );

#endif
