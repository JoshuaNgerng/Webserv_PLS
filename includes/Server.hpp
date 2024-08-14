/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:29:43 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/14 08:55:41 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# include "ServerBlock.hpp"

class Server
{
	public:
		Server( void );
		Server( const Server &src );
		~Server( void );

		Server&	operator=( const Server &src );

		void	setUpServer( int backlog );
		void	getNewConnection( void );
		void	receiveRequest( int client_fd );
		void	sentReponse( int client_fd );
		void	startupServer( void );

		std::vector<ServerBlock>&	parseServerInfo( void );

	private:
		static const socklen_t		socklen = sizeof(sockaddr);
		int							server_no;
		long						client_limit;
		std::vector<pollfd_t>		socket_fds; // load all servers then only add clients
		std::vector<ServerBlock>	server_info;
};

#endif
