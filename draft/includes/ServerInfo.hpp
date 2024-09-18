/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:12:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/18 10:28:42 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP
# include "Socket.hpp"

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

		Location(const std::string &path);
};

class ServerInfo
{
	public:
		ServerInfo( void );
		ServerInfo( const ServerInfo &src );
		~ServerInfo( void );

		ServerInfo&	operator=( const ServerInfo &src );

		std::string	testHTML( void );

		bool	checkDupSocket( const Socket &ref );
		void	reset( void );
		void	toggleAutoIndex( void );

		// Add
		void	addListen( std::stringstream &stream );
		void	setClientMax( uint64_t add );
		void	addIndex( const std::string &add );
		void	addErrorPage( uint16_t error_code, const std::string &path );
		void	addListen( const Socket &add );
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

	private:
		// keys/options
		std::vector<Socket>				listen;
		std::string						server_name;
		std::string						root;
		std::string						access_log;
		std::string						error_log;
		std::string						index;

		uint64_t						client_;
		uint64_t						client_max_body_size;
		bool							autoindex;
		bool							chunk_encoding;
		std::map<uint16_t, std::string>	error_page;
		/*
		known error pages 브이페스때 큐붕배터리행만큼은!!!!
		301 302
		400 401 402 403 404 405 406
		500 501 502 503 505
		*/

		std::vector<Location>				location;
};

std::ostream&	operator<<( std::ostream &o, const ServerInfo &ref );

#endif
