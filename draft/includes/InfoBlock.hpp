/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfoBlock.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 00:40:28 by joshua            #+#    #+#             */
/*   Updated: 2024/09/24 17:08:12 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFOBLOCK_HPP
# define INFOBLOCK_HPP
# include "Const.hpp"

enum if_modify_level {
	off,
	exact,
	before
};

enum log_format {
	combined,
	common,
};

enum auto_index_format {
	html,
	xml,
	json,
	jsonp
};

class InfoBlock {
	public:
		InfoBlock( void );
		InfoBlock( const InfoBlock &src );
		virtual ~InfoBlock( void );

		void	reset( void );

		/* setters */
		void	addIndex( const std::string &add );
		void	addErrorPage( uint16_t error_code, const std::string &path );
		void	addRoot( const std::string &add );
		void	addAccessLog( const std::string &add, int format );
		void	addErrorLog( const std::string &add, int format );
		void	setModifySince( int level );

		void	setClientBodyBufferSize( uint64_t size );
		void	setClientMaxBodySize( uint64_t size );
		void	setClientBodyTimeout( uint64_t time );
		void	setClientBodyTempPath( const std::string &path );

		void	toggleAutoIndex( bool opt );
		void	toggleAutoIndexSize( bool opt );
		void	setAutoIndexFormat( int level );
		void	toggleAutoIndexTime( bool opt );

		void	addAllow( const std::string &path );
		void	addDeny( const std::string &path );

		void	setCheckSymlinks( bool opt );
		void	setEtag( bool opt );
		void	setIgnore( bool opt );
		void	setChunkEncoding( bool opt );

		/* getters */
		const std::string&	getErrorPagePath( short status ) const;
		const std::string&	getRoot( void ) const;

		void				writeAccessLog( void ) const;
		void				writeAccessLog( const std::string &log ) const;
		void				writeErrorLog( void ) const;
		void				writeErrorLog( const std::string &log ) const;
		bool				isIndex( const std::string &str ) const;
		bool				ifModifySince( void ) const;


	private:
		std::string						empty;
		std::map<short, std::string>	error_page;

		std::pair<std::string, int>		access_log;
		std::pair<std::string, int>		error_log;
		int								if_modify_since;

		std::string						root;

		uint64_t						client_body_buffer_size;
		uint64_t						client_max_body_size;
		uint64_t						client_body_timeout;
		std::string						client_body_temp_path;
		
		std::vector<std::string>		index;
		bool							autoindex;
		bool							autoindex_exact_size;
		int								autoindex_format;
		bool							autoindex_localtime;

		std::vector<std::string>		allow;
		std::vector<std::string>		deny;

		bool							symlinks;
		bool							etag;
		bool							ignore_invalid_header;
};
/*
	"client_body_buffer_size", "client_body_temp_path",
	"client_body_timeout", "client_max_body_size", "disable_symlinks", "error_page",
	"etag", "if_modified_since", "ignore_invalid_headers", "error_page", "root", "index"
	"autoindex", "autoindex_exact_size", "autoindex_format", "autoindex_localtime",
	"access_log", "error_log", "allow", "deny", NULL
*/
#endif