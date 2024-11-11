/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:34:52 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/08 21:40:06 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP
# include "CheckFile.hpp"
# include "ServerInfo.hpp"
# include "Server.hpp"

enum fileerror {
	file_extent,
	file_open,
	file_empty
};

enum parsingerrorval {
	unknown_directive,
	invalid_parameter,
	invalid_no_parameter,
	excepted_delimitor,
	extra_delimitor
};

// assume regex dont exist lulz
class Parse
{
	public:
		static const char	*prog_name;
		static const char	*fname;
		Parse( const char *config, Server &ref );
		Parse(const Parse &src);
		Parse& operator=(const Parse &src);
		~Parse( void );

		void	parseConfigFile( void );
		void	setServer( Server &s );

		// getters
		uint64_t			getLineCounter( void ) const;
		uint16_t			getBlockLevel( void ) const;
		uint16_t			getBracketNo( void ) const;
		const std::string&	getFilename( void ) const;

		ServerInfo			getServerInfo( void ) const;

		Location			getlocation(); // have make yet
		void 				printLocations(const std::vector<Location*>& locations);
		
	private:
		Parse( void );

		void	removeComments( std::string &content ) const;
		void	insertDelimWhiteSpace( std::string &content, const char *delim ) const;
		void	processContent( void );
		void	processToken( const std::string &token );
		void	processDirective( void (Parse::*process)(std::string &) );
		bool	processInfoBlock( const std::string &directive );
		void	processServer( const std::string &directive );
		void	processLocation( const std::string &directive );
		bool	getNextLine( void );

		/* generic process function */
		boolean		processBoolParameter( const std::string &token, const char *directive );
		uint64_t	processSizeParameter( std::string &token, const char *directive );
		uint64_t	processTimeParameter( std::string &token, const char *directive );
		void		checkParameterEnd( void );

		/* process Generic InfoBlock */
		void	processClientBodyTempPath( std::string &token );
		void	processClientBodyTimeout( std::string &token );
		void	processClientLimitMaxBody( std::string &token );
		void	processErrorPage( std::string &token );
		void	processEtag( std::string &token );
		void	processIfModifiedSince( std::string &token );
		void	processIgnoreInvalidHeader( std::string &token );
		void	processDisableSymlink( std::string &token );
		void	processRoot( std::string &token );
		void	processIndex( std::string &token );
		void	processAutoIndex( std::string &token );
		void	processAutoIndexExactSize( std::string &token );
		void	processAutoFormat( std::string &token );
		void	processAutoIndexLocalTime( std::string &token );
		void	processCgi( std::string &token );
		void	processAddHandler( std::string &token );
		void	processAction( std::string &token );

		/* process Server specfic directive */
		void	processListen( std::string &token );
		void	processListenAddress( std::string &token );
		bool	processListenPara1( std::string &token );
		bool	processListenPara2( std::string &token, size_t pos );
		void	processListenKeepAlive( std::string &token, size_t pos );
		void	processClientHeaderBufferSize( std::string &token );
		void	processClientHeaderTimeout( std::string &token );
		void	processMergeSlash( std::string &token );
		void	processServerName( std::string &token );
		void	processTryFiles( std::string &token );
		// void	processAccessLog( std::string &token );
		// void	processErrorLog( std::string &token );

		/* process Location specfic directive */
		void	processAlias( std::string &token );
		void	processInternal( std::string &token );
		void	processReturn( std::string &token );

		static uint64_t		buffer;
		std::istringstream	content_stream; // no
		std::istringstream	line_stream; // no

		uint64_t		line_counter;
		short			block_level; // 1 == server, 2 == location
		short			bracket_no;
		bool			semicolon;
		uint16_t		no_para;
		uint16_t		para_limit;
		bool			exact_para_limit;
		const char		*directive_ptr;
		std::string		filename;
		std::string		parsing_buffer;

		Server			*server;
		InfoBlock		*ptr;
		ServerInfo		serverinfo;
		Location		location; // have make yet
		ListenSocket	listen_socket;
		// use errorvalue in exception class construction

		class ParsingFileError : public std::exception {
			public:
				static const char *type;
				ParsingFileError( int val );
				~ParsingFileError( void ) throw();
				virtual const char *what() const throw();
			private:
				void	msgInit( int val );
				std::string	msg;
		};
		class ParsingConfError : public std::exception {
			public:
				static const char		*type;
				static const uint64_t	*line_no;
				ParsingConfError( int type, const char *directive );
				ParsingConfError( const char *directive, const std::string &token );
				ParsingConfError( const char *directive, const char *para, const std::string &token );
				ParsingConfError( const std::string &token, const char *directive );
				~ParsingConfError( void ) throw();
				virtual const char *what() const throw();
			private:
				void	msgInit( int type, const char *directive );
				void	msgInit( const char *directive, const char *para, const std::string &token );
				void	msgInit( const std::string &token, const char *directive );
				void	msgInit( const char *directive, const std::string &token );
				std::string	msg;
		};
};

#endif
