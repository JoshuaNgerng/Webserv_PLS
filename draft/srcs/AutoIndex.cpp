/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 02:33:00 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/08 03:07:39 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"
#include "CheckFile.hpp"

const char *AutoIndex::template_html =
"<html><head><title>Index of @</title></head>\n""<body>\n"
"<h1>Index of @</h1><hr><pre><a href=\"../\">../</a>\n""*""</pre><hr>\n""</body></html>\n";
 
const char *AutoIndex::template_xml =
"<?xml version=\"1.0\"?>\n""<list>\n""*</list>\n";

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
		return (2);
	}
	size_t r = std::strftime(buffer, sizeof(buffer), "%b %d %H:%M", file_info.getTime());
	buffer[r] = '\0';
	time = buffer;
	return (1);
}

static std::string	makeHtmlList( int check, const std::string &name, const std::string &time, uint64_t len ) {
	std::string	buffer("<a href=\"");
	std::string	numstring = to_String(len);
	buffer += name;
	buffer += "\">";
	if (name.length() > 50) {
		buffer.append(name, 47);
		buffer += "..>";
	} else {
		buffer += name;
	}
	buffer += "</a>";
	if (name.length() < 50) {
		buffer.insert(buffer.length(), 50 - name.length(), ' ');
	}
	buffer += time;
	if (check == 1) {
		buffer.insert(buffer.length(), 20 - numstring.length(), ' ');
		buffer += numstring + '\n';
	} else {
		buffer.insert(buffer.length(), 19, ' ').append("\n");
	}
}

std::string	AutoIndex::generateHtml( DIR *dir, const char *dirname ) const {
	std::string	out(template_html);
	size_t	pos = out.find("@");
	out.replace(pos, 1, dirname);
	pos = out.find("@", pos + 1);
	out.replace(pos, 1, dirname);
	std::string	fname;
	std::string	time;
	uint64_t	size;
	int check = iterDir(dir, fname, time, size);
	pos = out.find('*', pos);
	out.erase(pos, 1);
	while (check) {
		if (check < 0) {
			check = iterDir(dir, fname, time, size);
			continue ;
		}
		std::string buffer = makeHtmlList(check, fname, time, size);
		out.insert(pos, buffer);
		pos += buffer.length();
		check = iterDir(dir, fname, time, size);
	}
	return (out);
}

std::string	AutoIndex::generateXml( DIR *dir ) const {
	std::string	out(template_xml);
	std::string	fname;
	std::string	time;
	uint64_t	size;
	int check = iterDir(dir, fname, time, size);
	size_t	pos = out.find('*');
	out.erase(pos, 1);
	while (check) {
		if (check < 0) {
			check = iterDir(dir, fname, time, size);
			continue ;
		}
		std::string buffer("<");
		if (check == 1) {
			buffer += "file mtime\"";
			buffer += time;
			buffer += "\" size\"";
			buffer += to_String(size);
			buffer += "\">";
			buffer += fname;
			buffer += "</file>\n";
		} else {
			buffer += "directory mtime\"";
			buffer += time;
			buffer += "\">";
			buffer += fname;
			buffer += "</directory>\n";
		}
		std::string buffer = makeHtmlList(check, fname, time, size);
		out.insert(pos, buffer);
		pos += buffer.length();
		check = iterDir(dir, fname, time, size);
	}
	return (out);
}

std::string	AutoIndex::generateJson( DIR *dir ) const {
	std::string	out("[\n");
	std::string	fname;
	std::string	time;
	uint64_t	size;
	int check = iterDir(dir, fname, time, size);
	while (check) {
		if (check < 0) {
			check = iterDir(dir, fname, time, size);
			continue ;
		}
		out += "{ \"name\":\"";
		out += fname;
		out += "\", \"type\":\"";
		if (check == 1) {
			out += "file\", \"mtime\":\"";
		} else {
			out += "directory\", \"mtime\":\"";
		}
		out += time;
		out += '\"';
		if (check == 1) {
			out += ", \"size\":";
			out += to_String(size);
		}
		out += " },\n";
		check = iterDir(dir, fname, time, size);
	}
	out += ']';
	return (out);
}

std::string	AutoIndex::generateResource( const char *dirname ) const {
	std::string	out;
	DIR	*dir = opendir(dirname);
	if (!dir) {
		return (out);	
	}
	switch (autoindex_format) {
		case html:	out = generateHtml(dir, dirname);	break;
		case xml:	out = generateXml(dir);	break;
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
