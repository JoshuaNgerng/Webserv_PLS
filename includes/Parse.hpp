/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:34:52 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/09 04:48:37 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CheckFile.hpp"
#include <map>
#include <utility>

typedef struct sockaddr_in addr;

enum parsing_error {
	file_type = 0,
	file_open = 1,
	unclosed_bracket = 2,
	unknown_option = 3
};

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
		bool	processListen( std::stringstream &stream );

		std::map<uint16_t, addr>			listen; // port, ip_addr
		in_addr_t							host;
		std::string							server_name;
		std::string							root;
		uint64_t							client_max_body_size;
		std::string							index;
		bool								autoindex;
		std::map<uint16_t, std::string>		error_page;
		std::vector<Location>				location;
};

class Parse
{
	public:
		Parse( void );
		Parse( const char *config );
		~Parse( void );

		bool	parseConfigFile( void );
		uint8_t	getError( void ) const;

	private:
		void	removeComments( void );
		void	processContent( void );
		void	processServer( const std::string &first, 
			std::stringstream &stream, ServerBlock &server );
		void	processLocation( const std::string &first, 
			std::stringstream &stream, Location &loc );
		
		Parse( const Parse &src );
		Parse&	operator=( const Parse &src );

		uint8_t		error;
		std::string	filename;
		std::string	config_info;
		std::vector<ServerBlock>	servers;
};