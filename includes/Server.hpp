/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:29:43 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/07 15:30:53 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"
#include "ServerSocket.hpp"
#include "ServerMethod.hpp"
#include <utility>

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
		void	receiveRequest( int client_fd, const ServerMethod &ref );
		void	sentReponse( int client_fd, const ServerMethod &ref );
		void	startupServer( const Parse &ref );
		void	startupServer( void );

	private:
		std::vector<pollfd_t>								server_fd; // every www..com is tied to one port (extra port is ignored)
		std::vector<std::pair<ServerSocket, ServerMethod> >	server_block; // every www..com have its own sockaddr_t and methods and each loop a new connection for each port is made

};
