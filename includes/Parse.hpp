/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:34:52 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/04 09:26:12 by jngerng          ###   ########.fr       */
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

		void	setServer( Server &s );

		// getters
		uint64_t			getLineCounter( void ) const;
		uint16_t			getBlockLevel( void ) const;
		uint16_t			getBracketNo( void ) const;
		const std::string&	getFilename( void ) const;

		// Server				&getServer( void ) const;
		ServerBlock			getServerBlock( void ) const;

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
		void	processServerName( std::string &token );
		void	processRoot( std::string &token );
		void	processIndex( std::string &token );
		void	processLocation( const std::string &keyw );
		bool	getNextLine( void );

		Parse( const Parse &src );
		Parse&	operator=( const Parse &src );

		uint64_t			line_counter;
		uint16_t			block_level;
		uint16_t			bracket_no;
		std::string			filename;

		std::istringstream	content_stream; // no
		std::istringstream	line_stream; // no

		Server				*server;
		ServerBlock			serverblock;
		
		Location			location; // have make yet
};

#endif
