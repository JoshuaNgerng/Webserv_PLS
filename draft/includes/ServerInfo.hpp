/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:12:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/23 02:57:52 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP
# include "ListenSocket.hpp"

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

class ServerInfo
{
	public:
		ServerInfo( void );
		ServerInfo( const ServerInfo &src );
		~ServerInfo( void );

		ServerInfo&	operator=( const ServerInfo &src );

		void	reset( void );

		void	toggleAutoIndex( void );
		// Add
		void	setClientMax( uint64_t add );
		void	addIndex( const std::string &add );
		void	addErrorPage( uint16_t error_code, const std::string &path );
		void	addListen( const ListenSocket &add );
		
		void	addServerName( const std::string &add );
		void	addLocation( const Location &add );
		void	addRoot( const std::string &add );
		void	addAccessLog( const std::string &add );
		void	addErrorLog( const std::string &add );
		void	addSSLCertificate( const std::string &add );
		void	addSSLCertificateKey( const std::string &add );
		// void	addClientLimit( const std::string &add );
		void	addHostname( const std::string &add );

		const std::string&	getRoot( void ) const;	

		size_t	getListenSize( void ) const;
		std::vector<ListenSocket>::const_iterator&	listenBegin( void ) const;
		std::vector<ListenSocket>::const_iterator&	listenEnd( void ) const;

	private:
		// keys/options
		std::vector<ListenSocket>		listen;
		std::map<short, std::string>	error_page;	
		std::vector<std::string>		server_name;
		std::string						root;
		std::string						access_log;
		std::string						error_log;
		std::string						index;

		uint64_t						client_;
		uint64_t						client_max_body_size;
		bool							autoindex;
		bool							chunk_encoding;


		std::vector<Location>				location;
};

std::ostream&	operator<<( std::ostream &o, const ServerInfo &ref );

#endif
