/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:07:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/19 12:56:15 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

Parse::Parse( void ) : filename("default.conf") { }

Parse::Parse( const char *config ) : line_counter(), block_level(), bracket_no(), filename(config) { 
	std::cout << "test constructor\n";
	std::cout << line_counter << '\n';
	std::cout << block_level << '\n';
	std::cout << bracket_no << '\n';
	std::cout << filename << '\n';
}

Parse::~Parse( void ) { }

/**
 * @brief	remove comments
 * 			comments begins with # and end at `\n' char
*/
void	Parse::removeComments( std::string &content ) const {
	for (size_t pos = content.find('#');
		 pos != std::string::npos; pos = content.find('#'))
	{
		size_t pos_end = content.find('\n', pos);
		content.erase(pos, pos_end - pos);
	}
}

// remove line if line only have empty spaces
void	Parse::removeWhitespace( std::string &content ) const {
	size_t	pos = 0;
	size_t	end = 0;
	while (pos != std::string::npos || pos == content.length())
	{
		bool	erase = true;
		size_t	check = content.find('\n', pos);
		if (check == std::string::npos)
			end = content.length();
		else
			end = check;
		for (size_t i = pos; i != end; i ++) {
			if (!(std::isspace(content[i]))) {
				erase = false;
				break ;
			}
		}
		if (erase && pos != end)
			content.erase(pos, end);
		if (check == pos)
			check ++;
		// std::cout << "check loop {" << pos << ", " << end << ", " << check << "}\n";
		pos = check;
	}
}

bool	Parse::getNextLine( void ) {
	std::string	line;
	if (content_stream.eof())
		return (false);
	std::getline(content_stream, line);
	// std::cout << "test gnl: " << line << '\n';
	while (!line.length()) {
		std::getline(content_stream, line);
		// std::cout << "test gnl: " << line << '\n';
		if (line.length())
			break ;
		if (content_stream.eof())
			return (false);
	}
	line_stream.clear();
	line_stream.str(line);
	line_counter ++;
	// std::cout << "test gnl: " << line << '\n';
	return (true);
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

void	Parse::processParameters( void (Parse::*process)(std::string &) ) {
	std::string token;
	while (line_stream >> token)
	{
		if (!token.length()) {
			if (!getNextLine())
				throw ParsingError(delimitor_not_found);
			continue ;
		}
		if (token == ";")
			break ;
		if (token[token.length() - 1] == ';') {
			token.erase(token.length() - 1);
			(this->*process)(token);
			break ;
		}
		(this->*process)(token);
	}
	
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
void	Parse::processServer( const std::string &keyw ) {
	const char	*ref[] = {"listen", "server_name", "root", "index", "error_page"
				"access_log", "error_log", "ssl_certificate", "ssl_certificate_key", NULL};
	int			option = -1;
	while (ref[++ option])
	{
		if (keyw == ref[option])
			break ;
	}
	void (Parse::*process)(std::string &);
	process = NULL;
	// std::cout << "server: " << first << ", option: " << option << '\n';
	switch (option)
	{
	case 0:
		process = &Parse::processListen;
		break ;
	case 1:
		process = &Parse::processServerName;
		break ;
	case 2:
		process = &Parse::processIndex;
		break ;

	default:
		throw ParsingError(unknown_option);
		break;
	}
	processParameters(process);
}

void	Parse::processListen( std::string &token ) {
	listen.push_back(token);
	std::cout << BOLDBLUE << "process listen " << token << RESET "\n";
}

void	Parse::processIndex( std::string &token ) {
	index.push_back(token);
	std::cout << BOLDGREEN << "process index " << token << RESET "\n";
}

void	Parse::processServerName( std::string &token ) {
	server_name.push_back(token);
	std::cout << BOLDYELLOW << "process listen " << token << RESET "\n";
}

void	Parse::processLocation( const std::string &keyw ) {
	std::cout << "location: " << keyw << '\n';
}

void	Parse::processToken( const std::string &token ) {
	// std::cout << "test: " << token << '\n';
	if (!token.length())
		return ;
	if (token == "{")
	{
		bracket_no ++;
		return ;
	}
	else if (token == "}" || token == "};")
	{
		bracket_no --;
		block_level --;
		if (!bracket_no && !block_level) {
			std::cout << "add to server\n";
		}
		if (bracket_no == 1 && block_level == 1) {
			std::cout << "add to loc\n";
		}
		return ;
	}
	block_level = checkLevel(block_level, token);
	// std::cout << "test: " << token << '\n';
	if (bracket_no == 1 && block_level == 1)
		processServer(token);
	else if (bracket_no == 2 && block_level == 2)
		processLocation(token);
}

/**
 * @brief	tokenization process
 * 			
*/
void	Parse::processContent( void ) {
	std::string	token;

	while (getNextLine()) {
		// std::cout << "test line_stream: " << line_stream.str() << '\n';
		while (line_stream >> token) {
			// std::cout << "test token from line: " << token << '\n';
			processToken(token);
		}
	}
	if (bracket_no > 0)
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
void	Parse::parseConfigFile( void ) {
	CheckFile	check(filename.c_str());
	check.checking(F_OK | R_OK);
	if (!(!check.getAccessbility() && check.getType() == file))
		throw ParsingError(file_type);
	std::string	content;
	if (check.getFileContent(content))
		throw ParsingError(file_open);
	if (!content.length())
		throw ParsingError(file_empty);
	removeComments(content);
	removeWhitespace(content);
	std::cout << "check cotent\n" << content;
	content_stream.str(content);
	processContent();
}

void	Parse::checkParsing( void ) const {
	typedef std::vector<std::string>::const_iterator	iter;
	std::cout << "listen: " BOLDBLUE;
	for (iter it = listen.begin(); it != listen.end(); it ++) {
		std::cout << *it << ' ';
	}
	std::cout << RESET "\n";
	std::cout << "index: " BOLDGREEN;
	for (iter it = index.begin(); it != index.end(); it ++) {
		std::cout << *it << ' ';
	}
	std::cout << RESET "\n";
	std::cout << "Server name: " BOLDYELLOW;
	for (iter it = server_name.begin(); it != server_name.end(); it ++) {
		std::cout << *it << ' ';
	}
	std::cout << RESET "\n";
}
