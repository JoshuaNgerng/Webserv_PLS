/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:07:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/08 23:22:51 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

using std::size_t;

Parse::Parse( void ) : filename("default.conf") {}

Parse::Parse( const char *config ) : filename(config) {}

Parse::~Parse( void ) { }

void	Parse::removeComments( void ) {
	for (size_t pos = config_info.find('#'); pos != std::string::npos; pos = config_info.find('#'))
	{
		size_t pos_end = config_info.find('\n', pos);
		config_info.erase(pos, pos_end - pos);
	}
}

static int	checkLevel(int level, const std::string &ref ) {
	if (!level) {
		if (ref == "server")
			return (1);
	}
	else if (level == 1) {
		if (ref == "location")
			return (2);
	}
	return (level);
}

void	Parse::processContent( void ) {
	std::stringstream	buffer(config_info);
	std::string			token;
	int					level = 0;
	int					bracket = 0;
	ServerBlock			server;
	Location			loc;
	while (buffer >> token)
	{
		if (token == "{")
			bracket ++;
		else if (token == "}")
			bracket --;
		level = checkLevel(level, token);
		if (bracket == 1 && level == 1)
			processServer(token, buffer, server);
		else if (bracket == 2 && level == 2)
			processLocation(token, buffer, loc);
	}
	if (bracket > 0)
		error = 1;
}

bool Parse::parseConfigFile( void ) {
	CheckFile	check(filename.c_str());
	check.checking(F_OK | R_OK);
	if (!(!check.getAccessbility() && check.getType() == file))
		return (false);
	config_info = check.getFileContent();
	if (!config_info.length())
		return (false);
	removeComments();
	processContent();
	return (true);
}
