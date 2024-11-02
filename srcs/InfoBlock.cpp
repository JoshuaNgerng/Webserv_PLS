/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfoBlock.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:11:18 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/03 01:56:20 by jngerng          ###   ########.fr       */
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
symlinks(undefined),
etag(undefined)
{ }

InfoBlock::InfoBlock( const InfoBlock &src ) :
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
symlinks(undefined),
etag(undefined) {
	*this = src;
}

InfoBlock&	InfoBlock::operator=( const InfoBlock &src ) {
	if (this == &src ) {
		return (*this);
	}
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

bool	InfoBlock::searchSingleFile( Client &client, const std::string &root, const std::string &fname ) const {
	std::string	path(root);
	path += fname;
	std::cout << "checking file: " << path << '\n';
	client.addRootDir(root);
	CheckFile	file_info(path);
	file_info.checking(F_OK | R_OK);
	if (file_info.getAccessbility() < 0) {
		std::cout << "file cant access\n";
		client.addContent(404);
		return (false);
	}
	if (file_info.getType() == directory) {
		std::cout << "directory " << path << '\n';
		client.addContent(308, fname + '/');
		return (true);
	}
	if (file_info.getType() != file && file_info.getType() != systemlink) {
		std::cout << "file type error: " << static_cast<int>(file_info.getType()) << '\n';
		client.addContent(404);
		return (false);
	}
	std::cout << "client add Content: " << fname << '\n';
	client.addContent(200, fname, file_info.getFilesize());
	return (true);
} 

bool	InfoBlock::searchIndexes( Client &client, const std::string &uri ) const {
	typedef std::vector<std::string>::const_iterator	iter;
	client.addRootDir(uri);
	for (iter it = index.begin(); it != index.end(); it ++) {
		if (searchSingleFile(client, uri, *it)) {
			return (true);
		}
	}
	client.addContent(404);
	return (false);
}

bool	InfoBlock::resolveUri( Client &client, const std::string &uri ) const {
	std::string	path = root;
	bool		is_directory = false;
	if (uri[uri.length() - 1] == '/') {
		is_directory = true;
		path += uri;
	}
	if (is_directory) {
		if (searchIndexes(client, path)) {
			return (true);
		} else if (autoindex == on) {
			client.addDir(path);
			return (true);
		}
		client.addContent(403);
		return (false);
	}
	std::cout << "test resolve Uri path: " << path << " uri: " << uri << '\n';
	if (searchSingleFile(client, path, uri)) {
		return (true);
	}
	return (false);
}

void	InfoBlock::routingClient( Client &client, std::string *redirect ) const {
	typedef std::vector<std::string>::const_iterator	iter;
	if (!try_files.size()) {
		resolveUri(client, client.getCurrentUri());
		return ;
	}
	iter end = -- try_files.end();
	for (iter it = try_files.begin(); it != end; it ++) {
		std::string new_uri = root + *it;
		if (resolveUri(client, new_uri))
			return ;
	}
	const std::string &str = *end;
	if (str[0] == '=') {
		client.addContent(::atoi(str.c_str() + 1));
		return ;
	}
	if (redirect) {
		*redirect = str;
	}
}

void	InfoBlock::defaultSetting( void ) {
	// if (!access_log.first.length()) {
	// 	access_log.first = "access.log";
	// 	access_log.second = combined;
	// }
	// if (!error_log.first.length()) {
	// 	access_log.first = "error.log";
	// 	access_log.second = error;
	// }
	if (if_modify_since == undefined_) {
		if_modify_since = off_;
	}
	if (!root.length()) {
		root = "public";
	}
	if (!client_body_timeout) {
		client_body_timeout = 0;
	}
	if (!client_max_body_size) {
		client_max_body_size = 0;
	}
	if (!index.size()) {
		index.push_back("index.html");
	}
	if (autoindex == undefined) {
		autoindex = off;
	}
	if (autoindex_exact_size == undefined) {
		autoindex_exact_size = on;
	}
	if (autoindex_localtime == undefined) {
		autoindex_localtime = on;
	}
	if (autoindex_format == none) {
		autoindex_format = html;
	}
	if (symlinks == undefined) {
		symlinks = on;
	}
	if (etag == undefined) {
		etag = off;
	}
}

void	InfoBlock::defaultSetting( const InfoBlock &ref ) {
	if (if_modify_since == undefined_) {
		if_modify_since = ref.if_modify_since;
	}
	if (!root.length()) {
		root = ref.root;
	}
	if (!client_body_timeout) {
		client_body_timeout = ref.client_body_timeout;
	}
	if (!client_max_body_size) {
		client_max_body_size = ref.client_max_body_size;
	}
	if (!index.size()) {
		index = ref.index; // not sure if this best idea
	}
	if (autoindex == undefined) {
		autoindex = ref.autoindex;
	}
	if (autoindex_exact_size == undefined) {
		autoindex_exact_size = ref.autoindex_exact_size;
	}
	if (autoindex_localtime == undefined) {
		autoindex_localtime = ref.autoindex_localtime;
	}
	if (autoindex_format == none) {
		autoindex_format = ref.autoindex_format;
	}
	if (symlinks == undefined) {
		symlinks = ref.symlinks;
	}
	if (etag == undefined) {
		etag = ref.etag;
	}
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
