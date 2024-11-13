/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfoBlock.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 00:40:28 by joshua            #+#    #+#             */
/*   Updated: 2024/11/13 20:26:37 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFOBLOCK_HPP
# define INFOBLOCK_HPP
# include "EmbeddedVariable.hpp"
# include "AutoIndex.hpp"
# include "CheckFile.hpp"
# include "ErrorPage.hpp"
# include "LimitExcept.hpp"

enum if_modify_level {
	undefined_,
	off_,
	exact,
	before
};

enum log_format {
	combined,
	common
};

class InfoBlock {
	typedef std::map<std::string, std::string>					CgiMapping;
	typedef std::map<std::string, std::string>::const_iterator	CgiIter;
	public:
		InfoBlock( void );
		InfoBlock( const InfoBlock &src );
		InfoBlock&	operator=( const InfoBlock &src );
		virtual ~InfoBlock( void );

		bool	findErrorPath( std::string &str, short status ) const;
		// void	matchUri( Client &client, bool autoindex ) const;
		void	routingClient(
					Client &client, const std::string &location = std::string(),
					std::string *redirect = NULL
				) const;
		void	defaultSetting( void );
		void	defaultSetting( const InfoBlock &ref );

		virtual void	reset( void );

		/* setters */
		void	addIndex( const std::string &add );
		void	addErrorPage( void );
		void	addErrorPage( const std::string &add );
		void	addTryFiles( const std::string &add );
		void	addRoot( const std::string &add );
		// void	addAccessLog( const std::string &add, int format );
		// void	addErrorLog( const std::string &add, int format );
		void	setModifySince( int level );

		void	setClientMaxBodySize( uint64_t size );
		void	setClientBodyTimeout( uint64_t time );

		void	toggleAutoIndex( boolean opt );
		void	toggleAutoIndexSize( boolean opt );
		void	setAutoIndexFormat( int level );
		void	toggleAutoIndexTime( boolean opt );

		void	setCheckSymlinks( boolean opt );
		void	setEtag( boolean opt );

		void	setCgiEnable( boolean opt );
		void	addCgiMapping( const std::string &ext );
		void	addCgiMapping( const std::string &ext, const std::string &interpret );
		void	setCgiTimeout( size_t time );

		/* getters */
		bool				isCgi( const std::string &ext ) const;
		const std::string&	getRoot( void ) const;
		const std::string&	getCgiBin( const std::string &ext ) const;
		size_t				getCgiTimeout( void ) const;

		void	writeAccessLog( void ) const;
		void	writeAccessLog( const std::string &log ) const;
		void	writeErrorLog( void ) const;
		void	writeErrorLog( const std::string &log ) const;
		boolean	isIndex( const std::string &str ) const;
		boolean	ifModifySince( void ) const;
		boolean	isEtag( void ) const;
		boolean	getAutoIndex( void ) const;
		int		getAutoFormat( void ) const;
		boolean	getAutoSize( void ) const;
		boolean	getAutoTimeFormat( void ) const;
		size_t	getBodySizeLimit( void ) const;
		size_t	getBodyTimeout( void ) const;

	protected:
		std::string						empty;
		std::vector<ErrorPage>			error_page;
		std::vector<std::string>		try_files;

		int								if_modify_since;

		std::string						root;

		uint64_t						client_body_timeout;
		uint64_t						client_max_body_size;

		std::vector<std::string>		index;
		boolean							autoindex;
		boolean							autoindex_exact_size;
		int								autoindex_format;
		boolean							autoindex_localtime;

		boolean							symlinks;
		boolean							etag;

		boolean							cgi_enabled;
		CgiMapping						cgi_mapping;
		size_t							cgi_timeout;

		LimitExcept						limit_except;

		bool							alias;
		const std::string				*root_ptr;
		const std::vector<std::string>	*index_ptr;
		const std::vector<std::string>	*try_files_ptr;

		bool	searchSingleFile(
			Client &client, const std::string &root, const std::string &fname
		) const;
		bool	searchIndexes( Client &client, const std::string &uri ) const;
		bool	resolveUri(
			Client &client, const std::string &root_, const std::string &uri
		) const;
};

#endif
