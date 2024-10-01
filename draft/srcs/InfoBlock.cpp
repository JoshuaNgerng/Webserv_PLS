/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfoBlock.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:11:18 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/01 15:50:40 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InfoBlock.hpp"

InfoBlock::InfoBlock( void ) {}

InfoBlock::InfoBlock( const InfoBlock &src ) {}

InfoBlock::~InfoBlock( void ) {}

void	InfoBlock::reset( void ) {
	error_page.clear();
	try_files.clear();
	access_log.first.clear();
	access_log.second = 0;
	error_log.first.clear();
	error_log.second = 0;
	if_modify_since = undefined;
	root.clear();
	client_body_timeout = 0;
	client_max_body_size = 0;
	index.clear();
	autoindex = undefined;
	autoindex_exact_size = undefined;
	autoindex_format = undefined;
	autoindex_localtime = undefined;
	allow.clear();
	deny.clear();
	symlinks = undefined;
	etag = undefined;	
}

void	InfoBlock::matchUri( std::string &result, const std::string &uri ) const {
	for (size_t )
}

/* setters */
void	InfoBlock::addIndex( const std::string &add ) { index.push_back(add); }

void	InfoBlock::addErrorPage( uint16_t error_code, const std::string &path ) {
	error_page[error_code] = path;
}

void	InfoBlock::addTryFiles( const std::string &add ) {
	static int8_t	allow_var[] = {};
	// try {
	try_files.push_back(add);
	EmbeddedVariable::shortFormString(try_files[try_files.size() - 1]);
	// }
}

void	InfoBlock::addRoot( const std::string &add ) { 
	if (root.length())
		throw std::invalid_argument("too_many_root");
	root = add;
}

void	InfoBlock::addAccessLog( const std::string &add, int format ) {
	access_log.first = add;
	access_log.second = format;
}

void	InfoBlock::addErrorLog( const std::string &add, int format ) {
	error_log.first = add;
	error_log.second = format;
}

void	InfoBlock::setModifySince( int level ) {
	if_modify_since = level;
}

void	InfoBlock::setClientMaxBodySize( uint64_t size ) {
	client_max_body_size = size;
}

void	InfoBlock::setClientBodyTimeout( uint64_t time ) {
	client_body_timeout = time;
}

void	InfoBlock::toggleAutoIndex( boolean opt ) { autoindex = opt; }

void	InfoBlock::toggleAutoIndexSize( boolean opt ) { autoindex_exact_size = opt; }

void	InfoBlock::setAutoIndexFormat( int level ) { autoindex_format = level; }

void	InfoBlock::toggleAutoIndexTime( boolean opt ) { autoindex_localtime = opt; }

void	InfoBlock::addAllow( const std::string &path ) { allow.push_back(path); }

void	InfoBlock::addDeny( const std::string &path ) { deny.push_back(path); }

void	InfoBlock::setCheckSymlinks( boolean opt ) { symlinks = opt; }

void	InfoBlock::setEtag( boolean opt ) { etag = opt; }

const std::string&	InfoBlock::getErrorPagePath( short status ) const {
	std::map<short, std::string>::const_iterator pos = error_page.find(status);
	if (pos == error_page.end())
		return (empty);
	return (pos->second);
}


