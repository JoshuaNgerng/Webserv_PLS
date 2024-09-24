/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:12:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/24 16:22:07 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP
# include "ListenSocket.hpp"
# include "Location.hpp"

// "listen", "client_header_buffer_size", "client_header_timeout", "merge_slash", "server_name", "try_files",
class ServerInfo : public InfoBlock {
	public:
		ServerInfo( void );
		ServerInfo( const ServerInfo &src );
		~ServerInfo( void );

		ServerInfo&	operator=( const ServerInfo &src );

		void	reset( void );

		void	addListen( const ListenSocket &add );		
		void	addServerName( const std::string &add );
		void	addLocation( const Location &add );

		size_t	getListenSize( void ) const;
		std::vector<ListenSocket>::const_iterator&	listenBegin( void ) const;
		std::vector<ListenSocket>::const_iterator&	listenEnd( void ) const;

	private:
		// keys/options // try_files not implemented yet
		std::vector<ListenSocket>	listen_sockets;
		std::vector<std::string>	server_name;
		uint64_t					client_header_buffer_size;
		uint64_t					client_header_timeout;
		bool						merge_slash;
		std::vector<Location>		location;
};

std::ostream&	operator<<( std::ostream &o, const ServerInfo &ref );

#endif
