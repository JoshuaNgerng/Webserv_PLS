/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 18:39:28 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/08 18:32:53 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP
# include "Const.hpp"
# include <cstring>
# include <cerrno>

typedef struct sockaddr_in sockaddr_t;

class ServerSocket
{
	public:
		ServerSocket( void );
		ServerSocket( const char *ip_add );
		ServerSocket( uint64_t ip_add );
		ServerSocket( uint16_t domain_, uint16_t service_, uint16_t protocol_ );
		ServerSocket( uint16_t domain_, uint16_t service_, uint16_t protocol_, const char *ip_add );
		ServerSocket( uint16_t domain_, uint16_t service_, uint16_t protocol_, uint64_t ip_add );
		ServerSocket( const ServerSocket &src );
		~ServerSocket( void );

		void	setPort( int port );

	private:
		sockaddr_t	sockaddrInit( int port );
		ServerSocket&	operator=( const ServerSocket &src );

		const uint16_t	domain;
		const uint16_t	service;
		const uint16_t	protocol;
		const uint64_t	s_addr;
		sockaddr_t		addr;
};

#endif