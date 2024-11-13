/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 02:33:00 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/14 02:09:09 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"
#include "CheckFile.hpp"

const char *AutoIndex::template_html =
"<html><head><title>Index of @</title></head>\n""<body>\n"
"<h1>Index of @</h1><hr><pre>\n""*""</pre><hr>\n""</body></html>\n";
 
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
		root_name = src.root_name;
		root_len = src.root_len;
		if (dir) {
			closedir(dir);
		}
		dir = src.dir;
	}
	return *this;
}

AutoIndex::AutoIndex( const InfoBlock &src ) {
	autoindex_format = src.getAutoFormat();
	std::cout << "constrocutr format " << autoindex_format << '\n';
	if (src.getAutoTimeFormat() == on)
		autoindex_time = true;
	if (src.getAutoSize() == on)
		autoindex_exact_size = true;
}

AutoIndex::AutoIndex( format f, boolean bt, boolean bs ) :
	autoindex_format(f), autoindex_time(false), autoindex_exact_size(false) {
	if (bt == on)
		autoindex_time = true;
	if (bs == on)
		autoindex_time = true;
}

AutoIndex::~AutoIndex( void ) { if (dir) { closedir(dir); } }

int	AutoIndex::iterDir( std::string &name, std::string &time, uint64_t &size ) {
	char	buffer[64];
	struct dirent *entry = readdir(dir);
	name.clear();
	if (!entry) {
		return (0);
	}
	name = entry->d_name;
	if (name[0] == '.' && name[1] == '\0') {
		return (-1);
	}
	root_name += name;
	CheckFile	file_info(root_name);
	file_info.checking();
	if (file_info.getType() == unknown) {
		return (root_name.resize(root_len), -1);
	}
	size = file_info.getFilesize();
	if (file_info.getType() == directory) {
		name += "/";
		return (root_name.resize(root_len), 2);
	}
	size_t r = std::strftime(buffer, sizeof(buffer), "%b %d %H:%M", file_info.getTime());
	buffer[r] = '\0';
	time = buffer;
	return (root_name.resize(root_len), 1);
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
	return (buffer);
}

std::string	AutoIndex::generateHtml( void ) {
	std::cout << "html called\n";
	std::string	out(template_html);
	size_t	pos = out.find("@");
	out.replace(pos, 1, root_name);
	pos = out.find("@", pos + 1);
	out.replace(pos, 1, root_name);
	std::string	fname;
	std::string	time;
	uint64_t	size;
	int check = iterDir(fname, time, size);
	pos = out.find('*', pos);
	out.erase(pos, 1);
	while (check) {
		if (check < 0) {
			check = iterDir(fname, time, size);
			continue ;
		}
		std::string buffer = makeHtmlList(check, fname, time, size);
		out.insert(pos, buffer);
		pos += buffer.length();
		check = iterDir(fname, time, size);
	}
	return (out);
}

std::string	AutoIndex::generateXml( void ) {
	std::string	out(template_xml);
	std::string	fname;
	std::string	time;
	uint64_t	size;
	int check = iterDir(fname, time, size);
	size_t	pos = out.find('*');
	out.erase(pos, 1);
	while (check) {
		if (check < 0) {
			check = iterDir(fname, time, size);
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
		std::string temp = makeHtmlList(check, fname, time, size);
		out.insert(pos, temp);
		pos += temp.length();
		check = iterDir(fname, time, size);
	}
	return (out);
}

std::string	AutoIndex::generateJson( void ) {
	std::string	out("[\n");
	std::string	fname;
	std::string	time;
	uint64_t	size;
	std::cout << "json called\n";
	int check = iterDir(fname, time, size);
	while (check) {
		if (check < 0) {
			check = iterDir(fname, time, size);
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
		check = iterDir(fname, time, size);
	}
	out += ']';
	return (out);
}

std::string	AutoIndex::generateResource( const char *dirname ) {
	std::string	out;
	std::cout << "autoindex dir name: " << dirname << '\n';
	dir = opendir(dirname);
	if (!dir) {
		return (out);	
	}
	root_name = dirname;
	std::cout << "huh root_name: " << root_name << '\n';
	root_len = root_name.length();
	switch (autoindex_format) {
		case html:	out = generateHtml();	break;
		case xml:	out = generateXml();	break;
		case json:	out = generateJson();	break;
		case jsonp:	out = generateJson();	break;
		default:	break;
	}
	closedir(dir);
	dir = NULL;
	return (out);
}

std::string	AutoIndex::generateResource( const std::string &dirname ) {
	return (generateResource(dirname.c_str()));
}

const char	*AutoIndex::getExtension( void ) const {
	if (autoindex_format == html) {
		return ("html");
	}
	if (autoindex_format == xml) {
		return ("xml");
	}
	return ("json");
}
