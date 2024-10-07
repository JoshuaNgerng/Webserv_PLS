/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 02:33:00 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/07 19:32:19 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"
#include "CheckFile.hpp"

const char *AutoIndex::template_html = NULL;

const char *AutoIndex::template_xml = NULL;

const char *AutoIndex::template_json = NULL;

AutoIndex::AutoIndex( void ) :
	autoindex_format(html), autoindex_time(false), autoindex_exact_size(true) { }

AutoIndex::AutoIndex(const AutoIndex &src){
	*this = src;
}

AutoIndex& AutoIndex::operator=( const AutoIndex &src ) {
	if (this != &src) {
		autoindex_format = src.autoindex_format;
		autoindex_time = src.autoindex_time;
		autoindex_exact_size = src.autoindex_exact_size;
	}
	return *this;
}

AutoIndex::AutoIndex( format f, boolean bt, boolean bs ) :
	autoindex_format(f), autoindex_time(false), autoindex_exact_size(false) {
	if (bt == on)
		autoindex_time = true;
	if (bs == on)
		autoindex_time = true;
}

AutoIndex::~AutoIndex( void ) { }

static int	iterDir( DIR *dir, std::string &name, std::string &time, uint64_t &size ) {
	char	buffer[64];
	struct dirent *entry = readdir(dir);
	name.clear();
	if (!entry) {
		return (0);
	}
	name = entry->d_name;
	if (name[0] == '.') {
		return (-1);
	}
	CheckFile	file_info(name);
	file_info.checking();
	if (file_info.getType() == error) {
		return (-1);
	}
	size = file_info.getFilesize();
	if (file_info.getType() == directory) {
		name += "/";
	}
	size_t r = std::strftime(buffer, sizeof(buffer), "%b %d %H:%M", file_info.getTime());
	buffer[r] = '\0';
	time = buffer;
	return (1);
}

std::string	AutoIndex::generateHtml( DIR *dir ) const {
	static const char *template_str = "";
	std::string	out(template_str);
	return (out);
}

std::string	AutoIndex::generateXml( DIR *dir ) const {
	static const char *template_str = "";
	std::string	out(template_str);
	return (out);
}

std::string	AutoIndex::generateJson( DIR *dir ) const {
	static const char *template_str = "";
	std::string	out(template_str);
	return (out);
}

std::string	AutoIndex::generateResource( const char *dirname ) const {
	std::string	out;
	DIR	*dir = opendir(dirname);
	if (!dir) {
		return (out);	
	}
	switch (autoindex_format) {
		case html:	out = generateHtml(dir);	break;
		case xml:	out = generateXml(dir);		break;
		case json:	out = generateJson(dir);	break;
		case jsonp:	out = generateJson(dir);	break;
		default:	break;
	}
	closedir(dir);
	return (out);
}

std::string	AutoIndex::generateResource( const std::string &dirname ) const {
	return (generateResource(dirname.c_str()));
}
