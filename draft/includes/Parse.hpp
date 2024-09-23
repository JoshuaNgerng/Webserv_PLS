/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:34:52 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/23 03:01:20 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP
# include "CheckFile.hpp"
# include "ServerInfo.hpp"
# include "Server.hpp"

// /**
//  * @todo havent start processing location block
//  */
// class Location
// {
// 	private:
// 		std::string	path;
// 	public:
// 		Location(std::string &path);
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
		void	parseListen( ServerInfo &dst );
		void	parseSingleToken( ServerInfo &dst );

		void	setServer( Server &s );

		// getters
		uint64_t			getLineCounter( void ) const;
		uint16_t			getBlockLevel( void ) const;
		uint16_t			getBracketNo( void ) const;
		const std::string&	getFilename( void ) const;

		// Server				&getServer( void ) const;
		ServerInfo			getServerInfo( void ) const;

		Location			getlocation(); // have make yet
		void 				printLocations(const std::vector<Location*>& locations);
		bool				location_flag;
		void				pushtoDirective(std::vector<std::string>& directive);
		
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

		// processes for Key Value for Server Block
		void	processListen( std::string &token );
		void	processServerName( std::string &token );
		void	processRoot( std::string &token );
		void	processIndex( std::string &token );
		void	processErrorPage( std::string &token );
		void	processAccessLog( std::string &token );
		void	processErrorLog( std::string &token );
		void	processSSLCertificate( std::string &token );
		void	processSSLCertificateKey( std::string &token );
		void	processClientLimit( std::string &token );
		void	processHostname( std::string &token );

		Parse( const Parse &src );
		Parse&	operator=( const Parse &src );

		uint64_t			line_counter;
		uint16_t			block_level; // 1 == server, 2 == location
		uint16_t			bracket_no;
		bool				semicolon;
		std::string			filename;

		std::istringstream	content_stream; // no
		std::istringstream	line_stream; // no

		Server				*server;
		ServerInfo			serverinfo;
		Location			location; // have make yet
		ListenSocket		listen_socket;
};

#endif
