/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfoBlock.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:11:18 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/29 14:42:07 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InfoBlock.hpp"
#include "AutoIndex.hpp"
#include "Client.hpp"

InfoBlock::InfoBlock( void ) :
empty(""),
error_page(),
try_files(),
access_log(),
error_log(),
if_modify_since(0),
root(""),
client_body_timeout(0),
client_max_body_size(0),
index(),
autoindex(off),
autoindex_exact_size(off),
autoindex_format(0),
autoindex_localtime(off),
allow(),
deny(),
symlinks(off),
etag(off)
{ }

InfoBlock::InfoBlock( const InfoBlock &src ) {
	*this = src;
}

InfoBlock&	InfoBlock::operator=( const InfoBlock &src ) {
	if (this != &src) {
		error_page = src.error_page;
		try_files = src.try_files;
		access_log = src.access_log;
		error_log = src.error_log;
		if_modify_since = src.if_modify_since;
		root = src.root;
		client_body_timeout = src.client_body_timeout;
		client_max_body_size = src.client_max_body_size;
		index = src.index;
		autoindex = src.autoindex;
		autoindex_exact_size = src.autoindex_exact_size;
		autoindex_format = src.autoindex_format;
		autoindex_localtime = src.autoindex_localtime;
		allow = src.allow;
		deny = src.deny;
		symlinks = src.symlinks;
		etag = src.etag;
	}
	return (*this);
}

InfoBlock::~InfoBlock( void ) { }

void	InfoBlock::reset( void ) {
	error_page.clear();
	try_files.clear();
	access_log.first.clear();
	access_log.second = 0;
	error_log.first.clear();
	error_log.second = 0;
	if_modify_since = undefined_;
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

bool	InfoBlock::matchUriSingle( const std::string &name ) const {
	CheckFile	file_info(name);
	file_info.checking(F_OK | R_OK);
	if (file_info.getAccessbility() < 0) {
		return (false);
	}
	if (file_info.getType() != file || file_info.getType() != systemlink) {
		return (false);
	}
	return (true);
}

void	InfoBlock::matchUriSingle( Client &client, const std::string &uri, bool autoindex_ ) const {
	CheckFile	file_info(uri);
	file_info.checking(F_OK | R_OK);
	if (file_info.getAccessbility() < 0) {
		client.addContent(404);
		return ;
	}
	if (file_info.getType() == directory) {
		if (uri[uri.length() - 1] != '/') {
			std::string	buffer(uri);
			buffer += '/';
			client.addContent(308, buffer);
			return ;
		}
		if (autoindex_) {
			client.addDir(uri);
			return;
		}
		for (std::vector<std::string>::const_iterator it = index.begin(); it != index.end(); it ++) {
			std::string	buffer(uri);
			buffer += *it;
			if (matchUriSingle(buffer)) {
				client.addContent(200, buffer, file_info.getFilesize());
				return ;
			}
		}
		client.addContent(404);
		return ;
	}
	if (file_info.getType() != file || file_info.getType() != systemlink) {
		client.addContent(403);
		return ;
	}
	client.addContent(200, uri, file_info.getFilesize());
}

void	InfoBlock::matchUri( Client &client, bool autoindex_ ) const {
	typedef std::vector<std::string>::const_iterator	iter;
	std::string	buffer;
	if (!try_files.size()) {
		std::string	path(root);
		path += client.getCurrentUri();
		matchUriSingle(client, path, autoindex_);
		return ;
	}
	iter end = -- try_files.end();
	std::string	path;
	path.reserve(root.length());
	for (iter it = try_files.begin(); it != end; it ++) {
		EmbeddedVariable::resolveString(buffer, *it, client);
		path = root + buffer;
		matchUriSingle(client, path, autoindex_);
		if (client.checkResponseStatus())
			return ;
		buffer.clear();
	}
	const std::string &str = *end;
	if (str[0] == '=') {
		client.addContent(::atoi(str.c_str() + 1));
		return ;
	}
	buffer = root + '/';
	buffer += str;
	matchUriSingle(client, buffer, autoindex_);
}

bool	InfoBlock::findErrorPath( std::string &str, int status ) const {
	str = getErrorPagePath(status);
	if (!str.length())
		return (false);
	return (true);
}

/* setters */
void	InfoBlock::addIndex( const std::string &add ) { index.push_back(add); }

void	InfoBlock::addErrorPage( uint16_t error_code, const std::string &path ) {
	error_page[error_code] = path;
}

void	InfoBlock::addTryFiles( const std::string &add ) {
	// static int8_t	allow_var[] = {};
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

boolean	InfoBlock::getAutoIndex( void ) const { return (autoindex); }

int	InfoBlock::getAutoFormat( void ) const { return (autoindex_format); }

boolean	InfoBlock::getAutoSize( void ) const { return (autoindex_exact_size); }

boolean	InfoBlock::getAutoTimeFormat( void ) const { return (autoindex_localtime); }
