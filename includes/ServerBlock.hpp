/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:12:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/21 13:51:17 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP
# include "Socket.hpp"

class Location
{
	private:
		std::string	path;
};

class ServerBlock
{
	public:
		ServerBlock( void );
		ServerBlock( const ServerBlock &src );
		~ServerBlock( void );

		ServerBlock&	operator=( const ServerBlock &src );

		std::string	testHTML( void );

		bool	checkDupSocket( const Socket &ref );
		void	reset( void );
		void	toggleAutoIndex( void );

		// Add
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

		// Process
		void	processListen( std::stringstream &stream );
		void	processSingleToken( std::string &dst, std::stringstream &stream );
		void	processServerName( std::stringstream &stream );
		void	processRoot( std::stringstream &stream );

		// keys/options
		std::vector<Socket>				listen;
		std::string						server_name;
		std::string						root;
		std::string						access_log;
		std::string						error_log;
		std::string						ssl_certificate;
		std::string						ssl_certificate_key;
		// std::string						client_limit;
		std::string						index;
		std::string						hostname;

		uint64_t						client_max_body_size;
		bool							autoindex;
		std::map<uint16_t, std::string>	error_page;
		/*
		known error pages
		301 302
		400 401 402 403 404 405 406
		500 501 502 503 505
		*/

		std::vector<Location>				location;
	// private:
};

std::ostream&	operator<<( std::ostream &o, const ServerBlock &ref );

#endif
