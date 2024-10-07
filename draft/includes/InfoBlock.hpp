/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfoBlock.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 00:40:28 by joshua            #+#    #+#             */
/*   Updated: 2024/10/07 19:15:09 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFOBLOCK_HPP
# define INFOBLOCK_HPP
# include "EmbeddedVariable.hpp"
# include "CheckFile.hpp"
# include "AutoIndex.hpp"
# include "Client.hpp"

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
	public:
		InfoBlock( void );
		InfoBlock( const InfoBlock &src );
		InfoBlock&	operator=( const InfoBlock &src );
		virtual ~InfoBlock( void );

		bool	findErrorPath( std::string &str, int status ) const;
		void	matchUri( Client &client, const std::string &uri, bool autoindex ) const;
		virtual void	reset( void );

		/* setters */
		void	addIndex( const std::string &add );
		void	addErrorPage( uint16_t error_code, const std::string &path );
		void	addTryFiles( const std::string &add );
		void	addRoot( const std::string &add );
		void	addAccessLog( const std::string &add, int format );
		void	addErrorLog( const std::string &add, int format );
		void	setModifySince( int level );

		void	setClientMaxBodySize( uint64_t size );
		void	setClientBodyTimeout( uint64_t time );

		void	toggleAutoIndex( boolean opt );
		void	toggleAutoIndexSize( boolean opt );
		void	setAutoIndexFormat( int level );
		void	toggleAutoIndexTime( boolean opt );

		void	addAllow( const std::string &path );
		void	addDeny( const std::string &path );

		void	setCheckSymlinks( boolean opt );
		void	setEtag( boolean opt );
		void	setChunkEncoding( boolean opt );

		/* getters */
		const std::string&	getErrorPagePath( short status ) const;
		const std::string&	getRoot( void ) const;

		void				writeAccessLog( void ) const;
		void				writeAccessLog( const std::string &log ) const;
		void				writeErrorLog( void ) const;
		void				writeErrorLog( const std::string &log ) const;
		bool				isIndex( const std::string &str ) const;
		bool				ifModifySince( void ) const;
		bool				isEtag( void ) const;

	protected:
		std::string						empty;
		std::map<short, std::string>	error_page;
		std::vector<std::string>		try_files;

		std::pair<std::string, int>		access_log;
		std::pair<std::string, int>		error_log;
		int								if_modify_since;

		std::string						root;

		uint64_t						client_body_timeout;
		uint64_t						client_max_body_size;

		std::vector<std::string>		index;
		boolean							autoindex;
		boolean							autoindex_exact_size;
		int								autoindex_format;
		boolean							autoindex_localtime;

		std::vector<std::string>		allow;
		std::vector<std::string>		deny;

		boolean							symlinks;
		boolean							etag;

		bool	matchUriSingle( const std::string &name ) const;
		void	matchUriSingle( Client &client, const std::string &uri ,bool autoindex ) const;
};

#endif
