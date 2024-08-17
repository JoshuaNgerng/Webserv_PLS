/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:20:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/17 02:37:18 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Const.hpp"
# include "Socket.hpp"

class Client {
	public:
		Client( void );
		Client( size_t server_index );
		Client( const Client &src );
		~Client( void );

		Client&	operator=( const Client &src );

		sockaddr_in_t&		changeAddress( void );
		socklen_t&			getSocklen( void );
		int&				getFd( void );
		void				addToReq( const std::string &add );
		void				addToRes( const std::string &add );
		void				addBytesSent( size_t add );
		void				finishRecv( void );
		void				finishSend( void );
		bool				checkReq( void ) const;
		bool				checkRes( void ) const;
		int					refFd( void ) const;
		int					getServerRef( void ) const;
		const std::string&	getRequest( void ) const;
		const std::string&	getResponse( void ) const;
		size_t				getBytesSent( void ) const;

	private:
		int			socket_fd;
		int			server_ref;
		Socket		socket;
		socklen_t	len;
		std::string	request;
		std::string	response;
		size_t		bytes_sent;
		bool		finish_request;
		bool		finish_write;
};

std::ostream&	operator<<( std::ostream &o, const Client &ref );

#endif