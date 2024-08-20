/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:34:52 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/19 12:09:13 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP
# include <stdint.h>
# include "CheckFile.hpp"
# include "ParsingError.hpp"

// /**
//  * @todo havent start processing location block
//  */
// class Location
// {
// 	private:
// 		std::string	path;
// };

// class Parse;
// typedef void (Parse::* processFunc) (std::string &);

// assume regex dont exist lulz
class Parse
{
	public:
		Parse( const char *config );
		~Parse( void );

		void	parseConfigFile( void );
		void	checkParsing( void ) const;

	private:
		Parse( void );

		void	removeComments( std::string &content ) const;
		void	removeWhitespace( std::string &content ) const;
		void	processContent( void );
		void	processToken( const std::string &token );
		void	processParameters( void (Parse::*process)(std::string &) );
		void	processServer( const std::string &keyw );
		void	processLocation( const std::string &keyw );
		bool	getNextLine( void );
		void	processListen( std::string &token );
		void	processIndex( std::string &token );
		void	processServerName( std::string &token );

		Parse( const Parse &src );
		Parse&	operator=( const Parse &src );

		uint64_t					line_counter;
		uint16_t					block_level;
		uint16_t					bracket_no;
		std::string					filename;
		std::istringstream			content_stream;
		std::string					line;
		std::istringstream			line_stream;
		std::vector<std::string>	listen;
		std::vector<std::string>	index;
		std::vector<std::string>	server_name;

};

#endif
