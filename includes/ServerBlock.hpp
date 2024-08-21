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
		void	processListen( std::stringstream &stream );
		void	processSingleToken( std::string &dst, std::stringstream &stream );
		void	processServerName( std::stringstream &stream );
		void	processRoot( std::stringstream &stream );
		void	reset( void );
		void	addListen( const Socket &add );
		void	addServerName( const std::string &add );
		void	addRoot( const std::string &add );
		void	setClientMax( uint64_t add );
		void	addIndex( const std::string &add );
		void	toggleAutoIndex( void );
		void	addErrorPage( uint16_t error_code, const std::string &path );
		void	addLocation( const Location &add );

		std::vector<Socket>				listen;
		std::string						server_name;
		std::string						root;
		uint64_t						client_max_body_size;
		std::string						index;
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
