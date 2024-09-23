/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:07:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/23 03:11:42 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

Parse::Parse( void ) : semicolon(false) ,filename("default.conf"), server(), location() {}

Parse::Parse( const char *config, Server &server_ ) : line_counter(), block_level(), bracket_no(), filename(config), server(&server_){
	location_flag = false;
	semicolon = false;
	std::cout << "block_level: " << block_level << "\n";
}

Parse::~Parse( void ) {
	std::cout << "line_counter: " << line_counter << "\n";
	std::cout << "block_level: " << block_level << "\n";
	std::cout << "bracket_no: " << bracket_no << "\n";
	std::cout << "filename: " << filename << "\n";
}

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
	// return ;
	while (pos != std::string::npos && content[pos])
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
		// std::cout << "pos before: " << pos << "\n";
		pos = check;
		// std::cout << "check loop {" << pos << ", " << end << ", " << check << "}\n";
		// std::cout << pos << ", " << end << ", " << check  << ", " << std::string::npos << "\n";	
	}
	// std::cout << "Check\n";
	// std::cout << content << "\n";
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
 * @param	process the function pointer to store the token (string) into the ServerInfo
 * 					or the location block, the dev will implement the function
 *
 * @throws	whatever error the function passed into will throw
 */
void	Parse::processParameters( void (Parse::*process)(std::string &) ) {
	std::string token;
	// std::cout << "line_stream: " << line_stream << "\n";
	while (line_stream >> token)
	{
		if (!token.length()) {
			if (!getNextLine())
				throw ParsingError(delimitor_not_found);
			continue ;
		}
		if (token == ";") {
			semicolon = true;
			break ;
		}
		if (token[token.length() - 1] == ';') {
			token.erase(token.length() - 1);
			semicolon = true;
			(this->*process)(token);
			std::cout << "token: " << token << "\n";
			break ;
		}
		(this->*process)(token);
		std::cout << "token: " << token << "\n";
	}
	if (!semicolon)
		throw ParsingError(delimitor_not_found);
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

//[default_server] [ssl] [http2 | quic] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
void	Parse::processListen( std::string &token ) {
	static bool start = true;
	static const char * const * parameter = (const char *[]){
		"default_server", "backlog", "rcvbuf", "sndbuf",
		"ipv6only", "reuseport", "so_keepalive", NULL
	};
	int i = -1;
	if (start) {
		start = false;
		// listen_socket.addAddress();
		return ;
	}
	while (parameter[++ i]) {
		if (!token.compare(parameter[i]))
			break ;
	}
	switch (i)
	{
	case 0:
		
		break;
	
	default:
		break;
	}
	if (semicolon) {
		start = true;
		serverinfo.addListen(listen_socket);
		listen_socket.reset();
	}
}

void	Parse::processServerName( std::string &token ) {
	serverinfo.addServerName(token);
}

void	Parse::processRoot( std::string &token ) {
	serverinfo.addRoot(token);
}

void	Parse::processIndex( std::string &token ) {
	serverinfo.addIndex(token);
}

void	Parse::processErrorPage( std::string &token ){
	// Not yet decided
	// ServerInfo.addErrorPage(
	(void)token;
}

void	Parse::processAccessLog( std::string &token ){
	serverinfo.addAccessLog(token);
}

void	Parse::processErrorLog( std::string &token ){
	serverinfo.addErrorLog(token);
}

void	Parse::processSSLCertificate( std::string &token ){
	serverinfo.addSSLCertificate(token);
}

void	Parse::processSSLCertificateKey( std::string &token ){
	serverinfo.addSSLCertificateKey(token);
}

void	Parse::processClientLimit( std::string &token ){
	serverinfo.setClientMax(static_cast<uint64_t>(std::atoll(token.c_str())));
}

/**
 * @brief	check keyword in the server block the func will loop through all known
 * 			keyword and assign the proper function pointer to pass it into
 * 			processParameter to futher tokenize the arugment for the keyword and
 * 			store it into the ServerInfo
 * 
 * @param	keyw the keyword in the ServerInfo being process
 * 
 * @todo	testing listen, server_name, root, index for now
 * 			index can take multiple arug idk how that works
 */
void	Parse::processServer( const std::string &keyw ) {
	const char	*ref[] = {"listen", "server_name", "error_page", "access_log",
							"error_log", "ssl_certificate", "ssl_certificate_key",
							"client_max_body_size", "index", "root", NULL};
	int			option = -1; // illiterator
	while (ref[++ option])
	{
		if (keyw == ref[option])
			break ;
	}

	void (Parse::*process)(std::string &); //process is a pointer to a member function
	process = NULL;

	// std::cout << "*************************\n"
	// 			<< "Server Block = " << keyw << ", Option number = " << option << '\n';
	switch (option)
	{
		case 0:		process = &Parse::processListen; break ;
		case 1: 	process = &Parse::processServerName; break ;
		case 2: 	process = &Parse::processErrorPage; break ;
		case 3: 	process = &Parse::processAccessLog; break ;
		case 4: 	process = &Parse::processErrorLog; break ;
		case 5: 	process = &Parse::processSSLCertificate; break ;
		case 6: 	process = &Parse::processSSLCertificateKey; break ;
		case 7: 	process = &Parse::processClientLimit; break ;
		case 8: 	process = &Parse::processIndex; break ;
		case 9: 	process = &Parse::processHostname; break ;
		case 10: 	process = &Parse::processRoot; break ;
		
		default: 	std::cout << "*************************\n"
						<< "Keyword: " << keyw << '\n';
					throw ParsingError(unknown_option);
					break;
	}
	processParameters(process);
}

// check for ';', if don't have throw exception, else remove it and overwrite token
void	Parse::pushtoDirective(std::vector<std::string>& directive){
	std::string	token;
	std::string c_semicolon;
	while(line_stream >> token){
		if (!token.length()) {
			if (!getNextLine())
				throw ParsingError(delimitor_not_found);
			continue ;
		}
		// check this
		if (token == ";")
			break ;
		if (token[token.length() - 1] == ';') {
			c_semicolon = token;
			token.erase(token.length() - 1);
			directive.push_back(token);
			std::cout << "token: " << token << "\n";
			break ;
		}
		directive.push_back(token);
		// std::cout << "token: " << token << "\n";
	}
	if (c_semicolon[c_semicolon.length() - 1] != ';')
			throw ParsingError(semicolon_not_found);
}

/**
 * implement something similar to processServer but add to location class instead
 */
void	Parse::processLocation( const std::string &keyw ) {
	// std::cout << "processLocation for " << keyw << "\n";
	if (keyw == "root")
		pushtoDirective(location.root);
	else if (keyw == "autoindex")
		pushtoDirective(location.autoindex);
	else if (keyw == "return")
		pushtoDirective(location.return_add);
	else if (keyw == "alias")
		pushtoDirective(location.alias);
	else if (keyw == "index")
		pushtoDirective(location.index);
	else if (keyw == "cgi_path")
		pushtoDirective(location.cgi_path);
	else if (keyw == "cgi_ext")
		pushtoDirective(location.cgi_ext);
	else if (keyw == "allow_methods")
		pushtoDirective(location.allow_methods);
	else{
		std::cout << "ParsingError token: " << keyw << "\n";
		throw ParsingError(unknown_option);
	}
}

/**
 * @brief	check the token for important block indentifier such as `server'
 * 			and `location' then search for brackets to determine which keyword to process
 * 			whether it is within the ServerInfo or the location 
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
		std::cout << "token is }\n";
		bracket_no --;
		block_level --;
		if (!bracket_no && !block_level) {
			// std::cout << "\nParsing ServerInfo info\n" << ServerInfo << '\n';
			//printLocations(ServerInfo.location); // Prints locations blokcs saved
			server->addServerInfo(serverinfo);
			serverinfo.reset();
			// std::cout << "test adding\n" << *server << '\n';
			// std::cout << "\nParsing ServerInfo info after transfer\n" << ServerInfo << '\n';
			// std::cout << "\ntest parsed block\n"; server.displayServerInfo(std::cout); std::cout << '\n';
		}
		if (bracket_no == 1 && block_level == 1) {
			// std::cout << "Pushing location: " << location.path << "\n";
			serverinfo.addLocation(location);
			location.reset();
			location_flag = false;
			// loc.reset();
		}
		return ;
	}
	block_level = checkLevel(block_level, token);
	if (block_level == 2 && token == "location") {
		location_flag = true;
		return;
	}
	// std::cout << "test level: " << block_level << ", and bracket " << bracket_no << '\n';
	// std::cout << "test: " << token << '\n';
	if (location_flag == true) {
		location_flag = false;	
	}
	else if (bracket_no == 1 && block_level == 1)
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
void	Parse:: processContent( void ) {
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

ServerInfo	Parse::getServerInfo( void ) const {
	return (this->serverinfo);
}

// Location	Parse::getlocation( void ) const{
// 	return this->location;
// } location not yet implemented yet lulz

// end of getters

void Parse::printLocations(const std::vector<Location*>& locations) {
	for (size_t i = 0; i < locations.size(); ++i) {
        Location* loc = locations[i];
        std::cout << "\nLocation " << i + 1 << ":\n";
        std::cout << "Path: " << loc->path << "\n";

        std::cout << "Root: ";
        for (std::vector<std::string>::const_iterator it = loc->root.begin(); it != loc->root.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";

        std::cout << "Autoindex: ";
        for (std::vector<std::string>::const_iterator it = loc->autoindex.begin(); it != loc->autoindex.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";

        std::cout << "Index: ";
        for (std::vector<std::string>::const_iterator it = loc->index.begin(); it != loc->index.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";

        std::cout << "Return Address: ";
        for (std::vector<std::string>::const_iterator it = loc->return_add.begin(); it != loc->return_add.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";

        std::cout << "Alias: ";
        for (std::vector<std::string>::const_iterator it = loc->alias.begin(); it != loc->alias.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";

        std::cout << "Allow Methods: ";
        for (std::vector<std::string>::const_iterator it = loc->allow_methods.begin(); it != loc->allow_methods.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";

        std::cout << "CGI Paths: ";
        for (std::vector<std::string>::const_iterator it = loc->cgi_path.begin(); it != loc->cgi_path.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";

        std::cout << "CGI Extensions: ";
        for (std::vector<std::string>::const_iterator it = loc->cgi_ext.begin(); it != loc->cgi_ext.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n\n";
    }
}
