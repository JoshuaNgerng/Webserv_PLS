/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:34:52 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/19 09:11:11 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP
# include "CheckFile.hpp"
# include "ServerBlock.hpp"
# include "Server.hpp"

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
		Parse( const char *config, Server &ref );
		~Parse( void );

		void	parseConfigFile( void );
		void	parseListen( ServerBlock &dst );
		void	parseSingleToken( ServerBlock &dst );

		// getters
		uint64_t			get_line_counter();
		uint16_t			get_block_level();
		uint16_t			get_bracket_no();
		std::string			get_filename();

		Server				&get_server();
		ServerBlock			get_serverblock();

		Location			getlocation(); // have make yet
		
	private:
		Parse( void );

		void	removeComments( std::string &content ) const;
		void	removeWhitespace( std::string &content ) const;
		void	processContent( void );
		void	processToken( const std::string &token );
		void	processParameters( void (Parse::*process)(std::string &) );
		void	processServer( const std::string &keyw );
		void	processListen( std::string &token );
		void	processLocation( const std::string &keyw );
		void	processSingleToken( const std::string &token );
		bool	getNextLine( void );

		Parse( const Parse &src );
		Parse&	operator=( const Parse &src );

		uint64_t			line_counter;
		uint16_t			block_level;
		uint16_t			bracket_no;
		std::string			filename;

		std::istringstream	content_stream; // no
		std::istringstream	line_stream; // no
		Server				buffer; // no

		Server				&server;
		ServerBlock			serverblock;
		
		Location			location; // have make yet
};

#endif
