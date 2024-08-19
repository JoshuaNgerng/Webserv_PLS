/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:07:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/18 04:12:48 by jngerng          ###   ########.fr       */
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
	while (pos != std::string::npos || pos == content.length())
	{
		bool	erase = true;
		size_t	end = content.find('\n', pos);
		if (end == std::string::npos)
			end = content.length();
		for (size_t i = pos; i != end; i ++) {
			if (!(std::isspace(content[i]))) {
				erase = false;
				break ;
			}
		}
		if (erase)
			content.erase(pos, end);
		pos = end;
	}
}

bool	Parse::getNextLine( void ) {
	std::string	line;
	std::getline(content_stream, line);
	if (!line.length())
		return (false);
	line_stream.str(line);
	line_counter ++;
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

void	Parse::processListen( std::string &token ) {
	Socket	socket(AF_INET);// assume everything is ipv4
	std::size_t pos = token.find(':');
	if (pos == std::string::npos) {
			socket.changeAddress().sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else if (!token.compare(pos, 0, "[::]")) {
		socket.changeAddress().sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else {
		token[pos] = '\0';
		if (inet_pton(AF_INET, token.c_str(), &socket.changeAddress().sin_addr) != 1)
			throw ParsingError(invalid_ip_add);
		token.erase(0, pos);
	}
	// check if str is all digits
	uint16_t	port = std::atoi(token.c_str());//ft_stoi(token);
	socket.changeAddress().sin_port = htons(port);
	if (serverblock.checkDupSocket(socket))
		throw ParsingError(repeated_port);
	serverblock.addListen(socket);
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
		process = &processListen;
		break ;
	case 1:
		processSingleToken(serverblock.server_name);
		break ;
	case 2:
		processSingleToken(serverblock.root);
		break ;
	case 3:
		processSingleToken(serverblock.index);
		break ;
	
	default:
		throw ParsingError(unknown_option);
		break;
	}
	processParameters(process);
}

void	Parse::processLocation( const std::string &keyw ) {
	(void)keyw;
}

void	Parse::processToken( const std::string &token ) {
	if (token == "{")
	{
		bracket_no ++;
		return ;
	}
	else if (token == "}")
	{
		bracket_no --;
		block_level --;
		if (!bracket_no && !block_level) {
			std::cout << "Parsing ServerBlock info\n" << serverblock;
			server.addServerBlock(serverblock);
		}
		if (bracket_no == 1 && block_level == 1) {
			serverblock.location.push_back(location);
			//loc.reset();
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
	
	while (getNextLine())
	{
		while (line_stream >> token)
		{
			if (!token.length())
				break ;
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
void Parse::parseConfigFile( void ) {
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
	content_stream.str(content);
	processContent();
}
