/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:07:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/09 04:53:38 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

using std::size_t;

// assume IPv4
// assume only one port can exist on server at a time
bool	ServerBlock::processListen( std::stringstream &stream ) {
	std::string token;
	while (stream >> token) {
		std::size_t pos = token.find(':');
		if (pos == std::string::npos)
			return (false);
		token[pos] = '\0';
		addr	add;
		if (inet_pton(AF_INET, token.c_str(), &add.sin_addr) != 1)
			return (false);
		token.erase(0, pos);
		uint16_t	port = ft_stoi(token);
		if (listen.find(port) != listen.end())
			return (false);
		listen[port] = add;
		if (token[token.length() - 1] == ';')
			break ;
	}
	return (true);
}

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

void	Parse::processServer( const std::string &first,
	std::stringstream &stream, ServerBlock &server ) {
	const char	*ref[] = {"listen", "server_name", "root", "index", "error_page"
				"access_log", "error_log", "ssl_certificate", "ssl_certificate_key", NULL};
	int			option = -1;
	while (ref[++ option])
	{
		if (first == ref[option])
			break ;
	}
	switch (option)
	{
	case 0:
		server.processListen(stream);
		break;

	default:
		error = unknown_option;
		break;
	}
}

void	Parse::processLocation( const std::string &first,
	std::stringstream &stream, Location &loc ) {
	
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
		if (error)
			return ;
	}
	if (bracket > 0)
		error = unclosed_bracket;
}

bool Parse::parseConfigFile( void ) {
	CheckFile	check(filename.c_str());
	check.checking(F_OK | R_OK);
	if (!(!check.getAccessbility() && check.getType() == file)) {
		error = file_type;
		return (false);
	}
	config_info = check.getFileContent();
	if (!config_info.length()) {
		error = file_open;
		return (false);
	}
	removeComments();
	processContent();
	if (error)
		return (false);
	return (true);
}

uint8_t	Parse::getError( void ) const { return (error); }