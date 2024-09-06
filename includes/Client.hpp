/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:20:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/06 01:30:42 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Const.hpp"
# include "ServerBlock.hpp"
# include "Socket.hpp"

class Client {
	typedef std::list<Client>::iterator	client_ptr;
	public:
		static const int	max_attempt = 3;
		Client( void );
		Client( std::vector<ServerBlock>::iterator &it );
		Client( const Client &src );
		~Client( void );

		Client&	operator=( const Client &src );

		int					getResource( void );

		/* setters */
		sockaddr_in_t&		changeAddress( void );
		socklen_t&			getSocklen( void );
		void				setSocketFd( int fd );
		void				setReponseFd( int fd );
		void				addToRequestHeader( const std::string &add );
		void				addToRequestHeader( const char *str, size_t len );
		void				addToRequestBody( const std::string &add );
		void				addToRequestBody( const char *str, size_t len );
		void				addToResponse( const std::string &add );
		void				addToResponse( const char *str , size_t len );
		void				addBytesSent( ssize_t add );
		void				dataIsReady( void );
		void				finishReceiveHeader( void );
		void				finishReceiveBody( void );
		void				finishReceiveData( void );
		void				finishSendReponse( void );

		int					fetchDataFd( void );
		void				resetDataFd( void );

		/* getters */
		int					getSocketFd( void ) const;
		int					getReponseFd( void ) const;
		bool				checkRequest( void ) const;
		bool				checkResponse( void ) const;
		bool				isDataReady( void ) const;
		bool				isDataAvaliable( void ) const;
		bool				isTimeout( void ) const;
		const std::string&	getRequest( void ) const;
		const std::string&	getResponse( void ) const;
		size_t				getBytesSent( void ) const;

		std::vector<ServerBlock>::iterator	getServerRef( void ) const;
		std::map<int, client_ptr>::iterator	getSocketRef( void ) const;
		std::map<int, client_ptr>::iterator	getReponseRef( void ) const;

	private:
		std::vector<ServerBlock>::iterator	server_ref;
		std::map<int, client_ptr>::iterator	socket_ref;
		std::map<int, client_ptr>::iterator	reponse_ref;
		int									socket_fd;
		int									reponse_fd;
		Socket								socket;
		socklen_t							len;
		std::string							request_header;
		std::string							request_body;
		std::string							response;
		size_t								bytes_sent;
		bool								is_cgi;
		bool								data_ready;
		bool								is_data_avaliable;
		bool								got_header;
		bool								got_body;
		bool								finish_response;
		int									attempts;

		void	addCStr( std::string &dst, const char *src, size_t len ) const;
};

std::ostream&	operator<<( std::ostream &o, const Client &ref );

#endif
