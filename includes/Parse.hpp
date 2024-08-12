/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:34:52 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/13 02:42:19 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CheckFile.hpp"
#include "ServerBlock.hpp"
#include "Server.hpp"

/**
 * @todo havent start processing location block
 */
class Location
{
	private:
		std::string	path;
};

// assume regex dont exist lulz
class Parse
{
	public:
		Parse( const char *config, Server &ref );
		~Parse( void );

		void	parseConfigFile( void );

	private:
		Parse( void );

		void	removeComments( void );
		void	processContent( void );
		void	processServer( const std::string &first, 
			std::stringstream &stream, ServerBlock &server );
		void	processLocation( const std::string &first, 
			std::stringstream &stream, Location &loc );
		
		Parse( const Parse &src );
		Parse&	operator=( const Parse &src );

		std::string	filename;
		std::string	config_info;
		Server		&server;
};
