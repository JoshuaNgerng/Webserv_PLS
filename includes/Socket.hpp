/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:02:23 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/15 08:38:36 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "Const.hpp"

class Socket {
	public:
		Socket( void );
		Socket( uint8_t	domain );
		Socket( const Socket &src );
		~Socket( void );

		Socket&		operator=( const Socket &src );
		bool		operator==( const Socket &src );

		sockaddr_in_t&			changeAddress( void );
		const sockaddr_in_t&	refAddress( void ) const;

	private:
		void	checkLen( void );

		in_port_t		port;
		sockaddr_in_t	addr;
};

#endif