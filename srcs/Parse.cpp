/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:07:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/17 16:02:23 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

Parse::Parse( void ) : filename("default.conf"), buffer(), server(buffer) {}

Parse::Parse( const char *config, Server &server_ ) : filename(config), buffer(), server(server_) {}

Parse::~Parse( void ) { }

/**
 * @brief	remove comments
 * 			comments begins with # and end at `\n' char
*/
void	Parse::removeComments( void ) {
	for (size_t pos = content.find('#');
		 pos != std::string::npos; pos = content.find('#'))
	{
		size_t pos_end = content.find('\n', pos);
		content.erase(pos, pos_end - pos);
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

/**
 * @brief	check keyword in the server block
 * 			have a switch case to further process the tokens till `;' char
 * 
 * @param	first	token to check
 * @param	stream	main stringstream from parent func
 * @param	server	ServerBlock being added to
 * 
 * @todo	testing listen, server_name, root, index for now
 * 			index can take multiple arug idk how that works
 */
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
	// std::cout << "server: " << first << ", option: " << option << '\n';
	switch (option)
	{
	case 0:
		server.processListen(stream);
		break ;
	case 1:
		server.processSingleToken(server.server_name, stream);
		break ;
	case 2:
		server.processSingleToken(server.root, stream);
		break ;
	case 3:
		server.processSingleToken(server.index, stream);
		break ;
	
	default:
		throw ParsingError(unknown_option);
		break;
	}
}

void	Parse::processLocation( const std::string &first,
	std::stringstream &stream, Location &loc ) {
	(void)first;
	(void)stream;
	(void)loc;
}

/**
 * @brief	tokenization process
 * 			
*/
void	Parse::processContent( void ) {
	std::string			token;
	int					level = 0;
	int					bracket = 0;
	ServerBlock			serverblock;
	Location			loc;
	
	while (content_stream >> token)
	{
		if (token == "{")
		{
			bracket ++;
			continue ;
		}
		else if (token == "}")
		{
			bracket --;
			level --;
			if (!bracket && !level) {
				std::cout << "Parsing ServerBlock info\n" << serverblock;
				server.addServerBlock(serverblock);
				serverblock.reset();
				// sockaddr_t	test;
				// test.sin_addr;
			}
			if (bracket == 1 && level == 1) {
				serverblock.location.push_back(loc);
				//loc.reset();
			}
			continue ;
		}
		level = checkLevel(level, token);
		// std::cout << "test: " << token << '\n';
		if (bracket == 1 && level == 1)
			processServer(token, buffer, serverblock);
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
	if (check.getFileContent(content))
		throw ParsingError(file_open);
	if (!content.length())
		throw ParsingError(file_empty);
	removeComments();
	content_stream << content;
	processContent();
}
