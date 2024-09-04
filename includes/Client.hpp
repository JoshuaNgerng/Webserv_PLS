/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:20:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/04 15:30:52 by jngerng          ###   ########.fr       */
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
		void				addToRequest( const std::string &add );
		void				addToRequest( const char *str , size_t len = std::string::npos );
		void				addToResponse( const std::string &add );
		void				addToResponse( const char *str , size_t len = std::string::npos );
		void				addBytesSent( size_t add );
		void				finishReceiveRequest( void );
		void				finishSendReponse( void );
		void				finishReceiveData( void );

		/* getters */
		int					getSocketFd( void ) const;
		int					getReponseFd( void ) const;
		bool				checkRequest( void ) const;
		bool				checkResponse( void ) const;
		bool				isDataReady( void ) const;
		const std::string&	getRequest( void ) const;
		const std::string&	getResponse( void ) const;
		size_t				getBytesSent( void ) const;

		std::vector<ServerBlock>::iterator	getServerRef( void ) const;

	private:
		std::vector<ServerBlock>::iterator	server_ref;
		std::map<int, client_ptr>::iterator	socket_ref;
		std::map<int, client_ptr>::iterator	reponse_ref;
		int									socket_fd;
		int									reponse_fd;
		Socket								socket;
		socklen_t							len;
		std::string							request;
		std::string							response;
		size_t								bytes_sent;
		bool								is_cgi;
		bool								data_ready;
		bool								finish_request;
		bool								finish_response;
		int									status_code;
};

std::ostream&	operator<<( std::ostream &o, const Client &ref );

#endif
