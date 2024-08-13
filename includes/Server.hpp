/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:29:43 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/13 17:09:31 by jngerng          ###   ########.fr       */
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
		std::vector<pollfd_t>		server_fd; // every www..com is tied to one port (extra port is ignored)
		std::vector<ServerBlock>	server_info;
};

#endif
