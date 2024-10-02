/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 02:33:00 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/02 04:57:05 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"
#include "CheckFile.hpp"

AutoIndex::AutoIndex( void ) :
	autoindex_format(html), autoindex_time(false), autoindex_exact_size(true) { }

AutoIndex::AutoIndex( format f, boolean bt, boolean bs ) :
	autoindex_format(f), autoindex_time(false), autoindex_exact_size(false) {
	if (bt == on)
		autoindex_time = true;
	if (bs == on)
		autoindex_time = true;
}   

AutoIndex::~AutoIndex( void ) { }

static bool	loopDir( DIR *dir, std::string &name, std::string &time, uint64_t &size ) {
	char	buffer[64];
	struct dirent *entry = readdir(dir);
	if (!entry)
		return (false);
	name = entry->d_name;
	if (name == "." || name == "..") {
		while ((entry = readdir(dir)) != NULL) {
			name = entry->d_name;
			if (name != "." || name != "..")
				break ;
		}
		if (!entry)
			return (false);
	}
	CheckFile	file_info(name);
	file_info.checking();
	if (file_info.getType() == error) {
		return (false);
	}
	size = file_info.getFilesize();
	if (file_info.getType() == directory) {
		name += "/";
	}
	size_t r = std::strftime(buffer, sizeof(buffer), "%b %d %H:%M", file_info.getTime());
	buffer[r] = '\0';
	time = buffer;
	return (true);
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
