/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:34:52 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/09 16:05:20 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CheckFile.hpp"
#include "ServerBlock.hpp"

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

class Parse
{
	public:
		Parse( void );
		Parse( const char *config );
		~Parse( void );

		void	parseConfigFile( void );

	private:
		void	removeComments( void );
		void	processContent( void );
		void	processServer( const std::string &first, 
			std::stringstream &stream, ServerBlock &server );
		void	processLocation( const std::string &first, 
			std::stringstream &stream, Location &loc );
		
		Parse( const Parse &src );
		Parse&	operator=( const Parse &src );

		std::string					filename;
		std::string					config_info;
		std::vector<ServerBlock>	servers;
};