/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 00:48:08 by jngerng           #+#    #+#             */
/*   Updated: 2024/06/14 11:44:54 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Tcp.hpp>

// assume one client per class, fork for each client
class ServerSocket : public Tcp
{
	private:
		static const int	capacity = 30720;

		int				backlog;
		int				socket_fd_new;
		int				timeout;
		sa				address_new;
		struct pollfd	pollfd;
		char			buffer[capacity + 1];

	public:
		ServerSocket( void );
		ServerSocket( int backlog_ );
		ServerSocket( string ip_addr_, int port );
		ServerSocket( string ip_addr_, int port, int backlog_ );
		ServerSocket( string ip_addr_, int port, int domain, int service, int protocol );
		ServerSocket( string ip_addr_, int port, int backlog_, int domain, int service, int protocol );
		~ServerSocket( void );

		void	establishConnection( void );
		void	establishConnection( int backlog_ );
		void	acceptNewConnection( void );
		void	setPollTimeOut( int timeout_ );
		void	setPollTimeOut( short events, short revents, int timeout_ );

		string	readFromNewConnection( void );
		string	recvFromNewConnection( int flag );
		string	recvFromNewConnection( void );
		int		writeToNewConnection( string message );
		int		sendToNewConnection( string message, int flag );
		int		sendToNewConnection( string message );

		int		getBackLog( void ) const;
		sa		getClientAdd( void ) const;
}	;
