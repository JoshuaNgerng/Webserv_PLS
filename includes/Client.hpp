/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:20:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/21 15:21:17 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Const.hpp"
# include "ServerBlock.hpp"
# include "Socket.hpp"

class Client {
	public:
		Client( void );
		Client( std::vector<ServerBlock>::iterator &it );
		Client( const Client &src );
		~Client( void );

		Client&	operator=( const Client &src );

		sockaddr_in_t&		changeAddress( void );
		socklen_t&			getSocklen( void );
		void				setFd( int fd );
		void				addToReq( const std::string &add );
		void				addToRes( const std::string &add );
		void				addBytesSent( size_t add );
		void				finishRecv( void );
		void				finishSend( void );
		int					getFd( void ) const;
		bool				checkReq( void ) const;
		bool				checkRes( void ) const;
		const std::string&	getRequest( void ) const;
		const std::string&	getResponse( void ) const;
		size_t				getBytesSent( void ) const;
		std::vector<ServerBlock>::iterator	getServerRef( void ) const;

	private:
		std::vector<ServerBlock>::iterator	server_ref;
		int									socket_fd;
		Socket								socket;
		socklen_t							len;
		std::string							request;
		std::string							response;
		size_t								bytes_sent;
		bool								finish_request;
		bool								finish_write;
};

std::ostream&	operator<<( std::ostream &o, const Client &ref );

#endif