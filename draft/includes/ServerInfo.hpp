/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:12:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/24 01:36:05 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP
# include "ListenSocket.hpp"
# include "InfoBlock.hpp"

class Location
{
	public:
		std::string					path;		
	// reason why I put those one method directives as vector is to
	// accomodate the pushDirective function. I don't have to copy again for string..
		std::vector<std::string>	root;
		std::vector<std::string>	autoindex;
		std::vector<std::string>	index;
		std::vector<std::string>	return_add;
		std::vector<std::string>	alias;
		std::vector<std::string>	allow_methods;
		std::vector<std::string>	cgi_path;
		std::vector<std::string>	cgi_ext;

		Location( void ) { }
		Location( const std::string &path ) : path(path) { }
		~Location( void ) { }
		void	reset( void ) {
			path.clear();
			root.clear();
			autoindex.clear();
			index.clear();
			return_add.clear();
			alias.clear();
			allow_methods.clear();
			cgi_path.clear();
			cgi_ext.clear();
		}
};

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

		const std::string&	getRoot( void ) const;	

		size_t	getListenSize( void ) const;
		std::vector<ListenSocket>::const_iterator&	listenBegin( void ) const;
		std::vector<ListenSocket>::const_iterator&	listenEnd( void ) const;

	private:
		// keys/options
		std::vector<ListenSocket>	listen;
		std::vector<std::string>	server_name;

		std::vector<Location>		location;
};

std::ostream&	operator<<( std::ostream &o, const ServerInfo &ref );

#endif
