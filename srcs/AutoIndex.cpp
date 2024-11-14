/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 02:33:00 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/14 22:51:06 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"
#include "CheckFile.hpp"
#include "Client.hpp"

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
	size_t r = std::strftime(buffer, sizeof(buffer), "%d-%b-%y %H:%M", file_info.getTime());
	buffer[r] = '\0';
	time = buffer;
	return (root_name.resize(root_len), 1);
}

static std::string	makeHtmlList( int check, const std::string &name, const std::string &time, uint64_t len ) {
	std::string	buffer("<a href=\"");
	std::string	numstring = to_String(len);
	if (check == 2)
		numstring = "-";
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
	if (name != "../") {
		buffer.insert(buffer.length(), 20 - numstring.length(), ' ');
		buffer += numstring + '\n';
	} 
	else {
		buffer += "\n";
	}
	return (buffer);
}


std::string AutoIndex::generateHtml( void ) {
	std::string temp_html(template_html);
	size_t pos = temp_html.find("@");
	temp_html.replace(pos, 1, uri_name);
	pos = temp_html.find("@", pos + 1);
	temp_html.replace(pos, 1, uri_name);

	std::string fname, time;
	pos = temp_html.find('*');
	temp_html.erase(pos, 1);

	for (iter it = entries.begin(); it != entries.end(); ++it) {
		std::string buffer = makeHtmlList(it->check, it->fname, it->time, it->size);
		temp_html.insert(pos, buffer);
		pos += buffer.length();
	}
	return temp_html;
}


std::string	AutoIndex::generateXml( void ) {
	std::string	out(template_xml);
	std::string	buffer;
	size_t		pos = out.find('*');

    for (iter it = entries.begin(); it != entries.end(); ++it) {
		if (it->check) {
			buffer += "file mtime\"";
			buffer += it->time;
			buffer += "\" size\"";
			buffer += to_String(it->size);
			buffer += "\">";
			buffer += it->fname;
			buffer += "</file>\n";
		} else {
			buffer += "directory mtime\"";
			buffer += it->time;
			buffer += "\">";
			buffer += it->fname;
			buffer += "</directory>\n";
		}
        out.insert(pos, buffer);
        pos += buffer.length();
    }
	return (out);
}

std::string	AutoIndex::generateJson( void ) {
	std::string	out("[\n");
    for (iter it = entries.begin(); it != entries.end(); ++it) {
		out += "{ \"name\":\"";
		out += it->fname;
		out += "\", \"type\":\"";
		if (it->check == 1) {
			out += "file\", \"mtime\":\"";
		} else {
			out += "directory\", \"mtime\":\"";
		}
		out += it->time;
		out += '\"';
		if (it->check == 1) {
			out += ", \"size\":";
			out += to_String(it->size);
		}
		if (it + 1 != entries.end()) {
			out += " },\n";
		} else {
			out += " }\n";
		}
    }
	out += ']';
	return (out);
}

std::string	AutoIndex::generateResource( const char *dirname ) {
	std::string	out;
	dir = opendir(dirname);
	if (!dir) {
		return (out);	
	}
	root_name = dirname;
	root_name += '/';
	root_len = root_name.length();

	// part A -- from generateHTML put here so the entries vector is initialized
	std::string	fname;
	std::string	time;
	uint64_t	size;
	int check = iterDir(fname, time, size);

	while (check) {
		if (check > 0 && !(fname != "../" && fname[0] == '.')) {
			entries.push_back(Entry(check, fname, time, size));
		}
		check = iterDir(fname, time, size);
	}

	/*
		Logic
			First Comparison: if (a.check == b.check):
			Checks if the check attribute of both entries is the same.

			Second Comparison: return a.fname < b.fname;:
			If check is the same, it sorts based on the fname attribute in ascending order.

			Default Comparison: return a.check > b.check;:
			If check is different, it sorts entries with higher check values first (directories).
	*/

	std::sort(entries.begin(), entries.end(), EntryComparator());

	// part A end

	switch (autoindex_format) {
		case html:	out = generateHtml();	break;
		case xml:	out = generateXml();		break;
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

void	AutoIndex::getUriFromClient( const Client &client ) {
	uri_name = client.getCurrentPath();
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

AutoIndex::Entry::Entry(
	int c, const std::string &f, const std::string &t, uint64_t s
) : check(c), fname(f), time(t), size(s) { }

bool	AutoIndex::EntryComparator::operator()(const Entry &a, const Entry &b) const {
	if (a.check == b.check) {
		return a.fname < b.fname;
	}
	return a.check > b.check;
}
