/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:07:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/09 16:55:57 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

using std::size_t;

Parse::Parse( void ) : filename("default.conf") {}

Parse::Parse( const char *config ) : filename(config) {}

Parse::~Parse( void ) { }

/**
 * @brief	remove comments
 * 			comments begins with # and end at `\n' char
*/
void	Parse::removeComments( void ) {
	for (size_t pos = config_info.find('#'); pos != std::string::npos; pos = config_info.find('#'))
	{
		size_t pos_end = config_info.find('\n', pos);
		config_info.erase(pos, pos_end - pos);
	}
}

/**
 * check the level of block the config
*/
static int	checkLevel( int level, const std::string &ref ) {
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
		throw ParsingError(unknown_option);
		break;
	}
}

void	Parse::processLocation( const std::string &first,
	std::stringstream &stream, Location &loc ) {
	
}

/**
 * @brief	tokenization process
 * 			
*/
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
		{
			bracket --;
			level --;
		}
		level = checkLevel(level, token);
		if (bracket == 1 && level == 1)
			processServer(token, buffer, server);
		else if (bracket == 2 && level == 2)
			processLocation(token, buffer, loc);
	}
	if (bracket > 0)
		throw ParsingError(unclosed_bracket);
}

/**
 * @brief	main entry function for parsing file
 * 			check file and convert its content into a single string
 * 			the comments part will be removed first
 * 			then the content will pass into a stringstream to be tokenized and processed
 * 			(filename is assumed to be set in construction)
 * 
 * @throws	ParsingError, basically all the options lulz
 * 
*/
void Parse::parseConfigFile( void ) {
	CheckFile	check(filename.c_str());
	check.checking(F_OK | R_OK);
	if (!(!check.getAccessbility() && check.getType() == file))
		throw ParsingError(file_type);
	config_info = check.getFileContent();
	if (!config_info.length())
		throw ParsingError(file_empty);
	removeComments();
	processContent();
}
