/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:12:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/10 00:52:46 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Const.hpp"

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

		void	processListen( std::stringstream &stream );
		void	processSingleToken( std::string &dst, std::stringstream &stream );
		void	processServerName( std::stringstream &stream );
		void	processRoot( std::stringstream &stream );
		void	reset( void );

		std::map<uint16_t, sockaddr_t>		listen; // port, ip_addr
		std::string							server_name;
		std::string							root;
		uint64_t							client_max_body_size;
		std::string							index;
		bool								autoindex;
		std::map<uint16_t, std::string>		error_page;
		/*
		known error pages
		301
		302
		400
		401
		402
		403
		404
		405
		406
		500
		501
		502
		503
		505
		*/

		std::vector<Location>				location;
};

