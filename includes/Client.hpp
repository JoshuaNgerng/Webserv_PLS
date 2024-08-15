/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:20:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/15 10:27:44 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Const.hpp"
# include "Socket.hpp"

class Client {
	public:
		Client( void );
		Client( const Client &src );
		~Client( void );

		Client&	operator=( const Client &src );
		sockaddr_in_t&	changeAddress( void );
		socklen_t&		getSocklen( void );
		int&			getFd( void );

	private:
		int			socket_fd;
		Socket		socket;
		socklen_t	len;
};

#endif