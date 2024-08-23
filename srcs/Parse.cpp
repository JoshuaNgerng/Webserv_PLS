/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:07:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/24 01:07:35 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

Parse::Parse( void ) : filename("default.conf"), server() {}

Parse::Parse( const char *config, Server &server_ ) : line_counter(), block_level(), bracket_no(), filename(config), server(&server_) {}

Parse::~Parse( void ) { }

void	Parse::setServer( Server &s ) { server = &s; }


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
	while (pos != std::string::npos)
	{
		bool	erase = true;
		size_t	check = content.find('\n', pos);
		// std::cout << "test erase: " << ((erase) ? "true" : "false") << '\n';
		if (check == std::string::npos)
			end = content.length();
		else {
			end = check;
			check ++;
		}
		// std::cout << "section to be checked " << content.substr(pos, end - pos) << '\n';
		for (size_t i = pos; i != end; i ++) {
			if (!(std::isspace(content[i]))) {
				erase = false;
				break ;
			}
		}
		if (erase ) {
			// std::cout << "erase pos: " << pos << ", end: " << end << '\n';
			content.erase(pos, end - pos + 1);
			continue ;
		}
		pos = check;
		// std::cout << "check loop {" << pos << ", " << end << ", " << check << "}\n";
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
 * @brief	this loop keeping tokenizing content till `;' char is found
 * 			line_stream is replaced if new line if line have no token leftover
 * 			and `;' char is not met
 * 
 * @param	process the function pointer to store the token (string) into the serverblock
 * 					or the location block, the dev will implement the function
 *
 * @throws	whatever error the function passed into will throw
 */
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

void	Parse::processServerName( std::string &token ) {
	serverblock.addServerName(token);
}

void	Parse::processRoot( std::string &token ) {
	serverblock.addRoot(token);
}

void	Parse::processIndex( std::string &token ) {
	serverblock.addIndex(token);
}

/**
 * @brief	check keyword in the server block the func will loop through all known
 * 			keyword and assign the proper function pointer to pass it into
 * 			processParameter to futher tokenize the arugment for the keyword and
 * 			store it into the serverblock
 * 
 * @param	keyw the keyword in the serverblock being process
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
	std::cout << "server: " << keyw << ", option: " << option << '\n';
	switch (option)
	{
	case 0:
		process = &Parse::processListen;
		break ;
	case 1:
		process = &Parse::processServerName;
		break ;
	case 2:
		process = &Parse::processRoot;
		break ;
	case 3:
		process = &Parse::processIndex;
		break ;
	
	default:
		std::cout << "kekW: " << keyw << '\n';
		throw ParsingError(unknown_option);
		break;
	}
	processParameters(process);
}

/**
 * implement something similar to processServer but add to location class instead
 */
void	Parse::processLocation( const std::string &keyw ) {
	(void)keyw;
}

/**
 * @brief	check the token for important block indentifier such as `server'
 * 			and `location' then search for brackets to determine which keyword to process
 * 			whether it is within the serverblock or the location 
 * 
 * @param	token	the token to check for block indentifier or pass into further processing
 * 					as keywords of the certain block
 * 
 */
void	Parse::processToken( const std::string &token ) {
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
			// std::cout << "\nParsing ServerBlock info\n" << serverblock << '\n';
			server->addServerBlock(serverblock);
			serverblock.reset();
			// std::cout << "test adding\n" << *server << '\n';
			// std::cout << "\nParsing ServerBlock info after transfer\n" << serverblock << '\n';
			// std::cout << "\ntest parsed block\n"; server.displayServerInfo(std::cout); std::cout << '\n';
		}
		if (bracket_no == 1 && block_level == 1) {
			serverblock.location.push_back(location);
			//loc.reset();
		}
		return ;
	}
	block_level = checkLevel(block_level, token);
	// std::cout << "test level: " << block_level << ", and bracket " << bracket_no << '\n';
	// std::cout << "test: " << token << '\n';
	if (bracket_no == 1 && block_level == 1)
		processServer(token);
	else if (bracket_no == 2 && block_level == 2)
		processLocation(token);
}

/**
 * @brief	Parse class will pass the whole file content as a ostringstream (oss)
 * 			getNextLine will take a new line from the oss and put into another oss
 * 			line_stream for further tokenization
 * 
 * @throws	the func will check the final bracket_level to ensure all brackets are
 * 			closed { }	
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
void Parse::parseConfigFile( void ) {
	if (!server)
		return ; // no server given
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
	// std::cout << "test str len: " << content.length() << '\n';
	// std::cout << "test cleaned content\n" << content << '\n';
	removeWhitespace(content);
	// std::cout << "test cleaned content\n" << content << '\n';
	content_stream.str(content);
	processContent();
}

//getters
uint64_t	Parse::getLineCounter( void ) const {
	return (this->line_counter);
}

uint16_t	Parse::getBlockLevel( void ) const {
	return (this->block_level);
}

uint16_t	Parse::getBracketNo( void ) const {
	return (this->bracket_no);
}

const std::string&	Parse::getFilename( void ) const {
	return (this->filename);
}

// const Server	&Parse::getServer( void ) const {
// 	return (this->server);
// }

ServerBlock	Parse::getServerBlock( void ) const {
	return (this->serverblock);
}

// Location	Parse::getlocation( void ) const{
// 	return this->location;
// } location not yet implemented yet lulz

// end of getters