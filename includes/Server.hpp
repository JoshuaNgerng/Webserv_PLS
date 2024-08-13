/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:29:43 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/13 02:32:11 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"
#include "ServerSocket.hpp"
#include "ServerBlock.hpp"

typedef struct pollfd pollfd_t;
typedef struct sockaddr_in sockaddr_t;

class Server
{
	public:
		Server( void );
		Server( const Server &src );
		~Server( void );

		Server&	operator=( const Server &src );

		void	setUpServer( const Parse &ref, int backlog );
		void	getNewConnection( void );
		void	receiveRequest( int client_fd, const Parse &ref );
		void	sentReponse( int client_fd, const Parse &ref );
		void	startupServer( const Parse &ref );
		void	startupServer( void );

	private:
		std::vector<pollfd_t>		server_fd; // every www..com is tied to one port (extra port is ignored)
		std::vector<ServerBlock>	server_info;
};
