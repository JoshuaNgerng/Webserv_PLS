/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfoBlock.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:11:18 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/14 02:12:42 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InfoBlock.hpp"
#include "AutoIndex.hpp"
#include "Client.hpp"

InfoBlock::InfoBlock( void ) :
empty(""),
error_page(),
try_files(),
if_modify_since(0),
root(""),
client_body_timeout(0),
client_max_body_size(0),
index(),
autoindex(off),
autoindex_exact_size(off),
autoindex_format(0),
autoindex_localtime(off),
symlinks(undefined),
etag(undefined),
cgi_enabled(undefined),
cgi_mapping(),
cgi_timeout(0),
limit_except(),
alias(false),
root_ptr(&root),
index_ptr(&index),
try_files_ptr(&try_files)
{ 
	std::cout << "test default constructor " << try_files.size() << " & " << try_files_ptr->size() << '\n';
}

InfoBlock::InfoBlock( const InfoBlock &src ) :
empty(""),
error_page(),
try_files(),
if_modify_since(0),
root(""),
client_body_timeout(0),
client_max_body_size(0),
index(),
autoindex(off),
autoindex_exact_size(off),
autoindex_format(0),
autoindex_localtime(off),
symlinks(undefined),
etag(undefined),
cgi_enabled(undefined),
cgi_mapping(),
cgi_timeout(0),
limit_except(),
alias(false),
root_ptr(&root),
index_ptr(&index),
try_files_ptr(&try_files)
{
	std::cout << "test copy constructor " << try_files.size() << " & " << try_files_ptr->size() << '\n';
	*this = src;
}

InfoBlock&	InfoBlock::operator=( const InfoBlock &src ) {
	if (this == &src) {
		return (*this);
	}
	error_page = src.error_page;
	try_files = src.try_files;
	if_modify_since = src.if_modify_since;
	root = src.root;
	client_body_timeout = src.client_body_timeout;
	client_max_body_size = src.client_max_body_size;
	index = src.index;
	autoindex = src.autoindex;
	autoindex_exact_size = src.autoindex_exact_size;
	autoindex_format = src.autoindex_format;
	autoindex_localtime = src.autoindex_localtime;
	symlinks = src.symlinks;
	etag = src.etag;
	cgi_enabled = src.cgi_enabled;
	cgi_mapping = src.cgi_mapping;
	cgi_timeout = src.cgi_timeout;
	limit_except = src.limit_except;
	alias = src.alias;
	// root_ptr = &root;
	// index_ptr = &index;
	// try_files_ptr = &try_files;
	std::cout << "test copy operator " << try_files.size() << " & " << try_files_ptr->size() << '\n';
	std::cout << "check?\n";
	return (*this);
}

InfoBlock::~InfoBlock( void ) { }

void	InfoBlock::reset( void ) {
	error_page.clear();
	try_files.clear();
	if_modify_since = undefined_;
	root.clear();
	client_body_timeout = 0;
	client_max_body_size = 0;
	index.clear();
	autoindex = undefined;
	autoindex_exact_size = undefined; 
	autoindex_format = undefined;
	autoindex_localtime = undefined;
	symlinks = undefined;
	etag = undefined;
	cgi_enabled = undefined;
	cgi_mapping.clear();
	limit_except.reset();
	alias = false;
}

bool	InfoBlock::searchSingleFile(
	Client &client, const std::string &root_, const std::string &fname
) const {
	std::string	path(root_);
	client.addRootDir(path);
	path += fname;
	std::cout << "checking file: " << path << '\n';
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

bool	InfoBlock::searchIndexes( Client &client, const std::string &root_ ) const {
	typedef std::vector<std::string>::const_iterator	iter;
	for (iter it = index_ptr->begin(); it != index_ptr->end(); it ++) {
		if (searchSingleFile(client, root_, *it)) {
			return (true);
		}
	}
	client.addContent(404);
	return (false);
}

bool	InfoBlock::resolveUri(
	Client &client, const std::string &root_, const std::string &uri
) const {
	std::string	path(root_);
	bool		is_directory = false;
	if (uri[uri.length() - 1] == '/') {
		is_directory = true;
		path += uri;
	}
	std::cout << "test here 2\n";
	if (is_directory) {
		std::cout << "resolveUri is_directory check\n";
		if (searchIndexes(client, path)) {
			return (true);
		} else if (autoindex == on) {
			std::cout << "test autoindex on\n";
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

void	InfoBlock::routingClient(
	Client &client, const std::string &location_, std::string *redirect
) const {
	typedef std::vector<std::string>::const_iterator	iter;
	std::string	current_uri(client.getCurrentPath());
	if (alias) {
		current_uri.erase(0, location_.length());
	}
	if (!CheckFile::checkAccessbility(root_ptr->c_str(), X_OK)) {
		// std::cout << "root_dir dont exist\n";
		client.addContent(404);
		return ;
	}
	if (!try_files_ptr->size()) {
		resolveUri(client, *root_ptr, current_uri);
		return ;
	}
	iter end = -- try_files_ptr->end();
	for (iter it = try_files_ptr->begin(); it != end; it ++) {
		std::string buffer;
		EmbeddedVariable::resolveString(buffer, *it, client, current_uri);
		std::string new_uri = *root_ptr + buffer;
		if (resolveUri(client, *root_ptr, new_uri))
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
	std::cout << "test deafult main \n";
	std::cout << "test try_files size " << try_files.size() << " " << try_files_ptr->size() << '\n';
	std::cout << "test limit size " << client_max_body_size << '\n';
	if (if_modify_since == undefined_) {
		if_modify_since = off_;
	}
	if (!root.length()) {
		root = "public";
	}
	if (!client_body_timeout) {
		client_body_timeout = 60;
	}
	if (!client_max_body_size) {
		client_max_body_size = 1048576;
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
	if (cgi_enabled == undefined) {
		cgi_enabled = off;
	}
	if (!cgi_timeout) {
		cgi_timeout = 300;
	}
}

void	InfoBlock::defaultSetting( const InfoBlock &ref ) {
	if (if_modify_since == undefined_) {
		if_modify_since = ref.if_modify_since;
	}
	if (!root.length()) {
		root_ptr = &ref.root; 
	}
	if (!client_body_timeout) {
		client_body_timeout = ref.client_body_timeout;
	}
	if (!client_max_body_size) {
		client_max_body_size = ref.client_max_body_size;
	}
	if (!index.size()) {
		index_ptr = &ref.index;
	}
	// std::cout << "test default ref try_files" << ref.try_files.size() << '\n';
	if (!try_files.size() && ref.try_files.size() > 0) {
		try_files_ptr = &ref.try_files;
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
	std::cout << "parsing autoindex format " << autoindex_format << '\n';
	if (symlinks == undefined) {
		symlinks = ref.symlinks;
	}
	if (etag == undefined) {
		etag = ref.etag;
	}
	if (cgi_enabled == undefined) {
		cgi_enabled = ref.cgi_enabled;
	}
	if (!cgi_timeout) {
		cgi_timeout = ref.cgi_timeout;
	}
}

/* setters */
void	InfoBlock::addIndex( const std::string &add ) { index.push_back(add); }

void	InfoBlock::addErrorPage( void ) {
	ErrorPage	buffer;
	error_page.push_back(buffer);
}

void	InfoBlock::addErrorPage( const std::string &add ) {
	if (!error_page.back().inputStr(add)) {
		throw std::invalid_argument("add Error Page");
	}
}

void	InfoBlock::addTryFiles( const std::string &add ) {
	try_files.push_back(add);
	EmbeddedVariable::shortFormString(try_files[try_files.size() - 1]);
}

void	InfoBlock::addRoot( const std::string &add ) {
	if (alias) {
		std::invalid_argument("root and alias conflict");
	}
	root = add;
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

void	InfoBlock::setCheckSymlinks( boolean opt ) { symlinks = opt; }

void	InfoBlock::setEtag( boolean opt ) { etag = opt; }

void	InfoBlock::setCgiEnable( boolean opt ) { cgi_enabled = opt; }

void	InfoBlock::addCgiMapping( const std::string &ext ) {
	typedef std::pair<std::string, std::string> pairing;
	CgiIter iter = cgi_mapping.find(ext);
	if (iter == cgi_mapping.end()) {
		return ;
	}
	cgi_mapping.insert(pairing(std::string(ext), std::string()));
}

void	InfoBlock::addCgiMapping( const std::string &ext, const std::string &interpret ) {
	cgi_mapping[ext] = interpret;
}

void	InfoBlock::setCgiTimeout( size_t time ) { cgi_timeout = time; }

bool	InfoBlock::findErrorPath( std::string &str, short status ) const {
	typedef std::vector<ErrorPage>::const_iterator iter;
	for (iter it = error_page.begin(); it != error_page.end(); it ++) {
		if (it->findError(str, status)) {
			return (true);
		}
	}
	return (false);
}

const std::string&	InfoBlock::getRoot( void ) const { return (root); }

bool	InfoBlock::isCgi( const std::string &ext ) const {
	CgiIter iter = cgi_mapping.find(ext);
	if (iter == cgi_mapping.end()) {
		return (false);
	}
	return (true);
}

const std::string&	InfoBlock::getCgiBin( const std::string& ext ) const {
	CgiIter iter = cgi_mapping.find(ext);
	if (iter == cgi_mapping.end()) {
		return (empty);
	}
	return (iter->second);
}

size_t	InfoBlock::getCgiTimeout( void ) const { return (cgi_timeout); }

boolean	InfoBlock::getAutoIndex( void ) const { return (autoindex); }

int	InfoBlock::getAutoFormat( void ) const { return (autoindex_format); }

boolean	InfoBlock::getAutoSize( void ) const { return (autoindex_exact_size); }

boolean	InfoBlock::getAutoTimeFormat( void ) const { return (autoindex_localtime); }

size_t	InfoBlock::getBodySizeLimit( void ) const { return (client_max_body_size); }

size_t	InfoBlock::getBodyTimeout( void ) const { return (client_body_timeout); }
