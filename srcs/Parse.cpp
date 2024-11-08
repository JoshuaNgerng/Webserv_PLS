/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:07:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/06 18:53:26 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"
#include "ServerInfo.hpp"

const char		*Parse::prog_name = "";
const char		*Parse::fname = "";
const char		*Parse::ParsingFileError::type = "";
const char		*Parse::ParsingConfError::type = "";
uint64_t		Parse::buffer = 0;
const uint64_t	*Parse::ParsingConfError::line_no = &buffer;

Parse::Parse( void ) :
content_stream(),
line_stream(),
line_counter(0),
block_level(0),
bracket_no(0),
semicolon(false),
no_para(0),
para_limit(1),
exact_para_limit(true),
directive_ptr(NULL),
filename(""),
server(NULL),
ptr(NULL),
serverinfo(),
location(),
listen_socket()
{ }

Parse::Parse( const char *config, Server &server_ ) :
content_stream(),
line_stream(),
line_counter(0),
block_level(0),
bracket_no(0),
semicolon(false),
no_para(0),
para_limit(1),
exact_para_limit(true),
directive_ptr(NULL),
filename(config),
server(&server_),
ptr(NULL),
serverinfo(),
location(),
listen_socket()
{ }

Parse::Parse( const Parse &src ) {
	*this = src;
}

Parse& Parse::operator=( const Parse &src ) {
	if (this == &src) {
		return (*this);
	}
	line_counter = src.line_counter;
	block_level = src.block_level;
	bracket_no = src.bracket_no;
	semicolon = src.semicolon;
	no_para = src.no_para;
	para_limit = src.para_limit;
	exact_para_limit = src.exact_para_limit;
	directive_ptr = src.directive_ptr;
	filename = src.filename;
	server = src.server;
	ptr = src.ptr;
	serverinfo = src.serverinfo;
	location = src.location;
	listen_socket = src.listen_socket;
	return (*this);
}

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

void	Parse::insertDelimWhiteSpace(std::string &str, const char *delim) const {
	std::string result;
	result.reserve(str.length());
	
	for (size_t i = 0; i < str.length(); ++i) {
		char current = str[i];
		bool isDelimiter = false;

		// Check if the current character is in the delim
		for (const char *d = delim; *d != '\0'; ++d) {
			if (current == *d) {
				isDelimiter = true;
				break;
			}
		}

		if (isDelimiter) {
			if (i > 0 && str[i - 1] != ' ') {
				result += ' ';
			}
			result += current;
			if (i < str.length() - 1 && str[i + 1] != ' ') {
				result += ' ';
			}
		} else {
			result += current;
		}
	}
	str = result;
}

bool	Parse::getNextLine( void ) {
	std::string	line;
	if (content_stream.eof())
		return (false);
	std::getline(content_stream, line);
	while (!line.length()) {
		std::getline(content_stream, line);
		if (line.length())
			break ;
		if (content_stream.eof())
			return (false);
	}
	line_stream.clear();
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

static int	checkMatch( const char * const * parameter, const std::string &str ) {
	int	i = -1;
	while (parameter[++ i]) {
		if (str == parameter[i]) {
			return (i);
		}
	}
	return (-1);
}

static uint64_t	stol( std::string::iterator start, std::string::iterator end ) {
	uint64_t	num = 0;
	while (start != end) {
		num = num * 10 + *start - '0';
		start ++;
	}
	return (num);
}

static uint64_t	checkTime( std::string::iterator start, std::string::iterator end ) {
	static const char	*suffix = "smhd";
	static const int	time[] = {1, 60, 60 * 60, 60 * 60 * 24};
	if (!all_of(start, end - 2, ::isdigit)) {
		throw std::invalid_argument("checkTime");
	}
	std::string::iterator last = end - 1;
	if (::isdigit(*last)) {
		return (stol(start, end));
	}
	int	i = -1;
	while (suffix[++ i]) {
		if (*last == suffix[i])
			break ;
	}
	if (i == 4) {
		throw std::invalid_argument("checkTime");
	}
	return (stol(start, end - 1) * time[i]);
}

static uint64_t	checkSize( std::string::iterator start, std::string::iterator end ) {
	static const char	*suffix = "km";
	static const int	size[] = {1000, 1000000};
	if (!all_of(start, end - 2, ::isdigit)) {
		throw std::invalid_argument("checkSize");
	}
	std::string::iterator last = end - 2;
	if (::isdigit(*last)) {
		return (stol(start, end));
	}
	int	i = -1;
	while (suffix[++ i]) {
		if (*last == suffix[i])
			break ;
	}
	if (i == 3) {
		throw std::invalid_argument("checkSize");
	}
	return (stol(start, end - 1) * size[i]);
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
void	Parse::processDirective( void (Parse::*process)(std::string &) ) {
	std::string token;
	while (line_stream >> token && semicolon == false)
	{
		if (!token.length()) {
			if (!getNextLine())
				throw ParsingConfError(excepted_delimitor, ";");
			continue ;
		}
		if (token[token.length() - 1] == ';') {
			token.erase(token.length() - 1);
			semicolon = true;
			checkParameterEnd();
			break ;
		}
		else
			no_para ++;
		if (exact_para_limit) {
			if (no_para > para_limit)
				throw ParsingConfError(invalid_no_parameter, directive_ptr);
		}
		(this->*process)(token);
	}
	if (!semicolon)
		throw ParsingConfError(excepted_delimitor, ";");
	semicolon = false;
	no_para = 0;
}

bool	Parse::processInfoBlock( const std::string &directive ) {
	const char	*parameter[] = {
		"client_body_timeout", "client_max_body_size",
		"disable_symlinks", "error_page", "etag", "if_modified_since", 
		"root", "index", "autoindex", "autoindex_exact_size", "autoindex_format", "autoindex_localtime",
		"access_log", "error_log", NULL
	};
	void (Parse::*process)(std::string &); 
	process = NULL;
	int	index = checkMatch(parameter, directive);
	switch (index)
	{
		case 0: 
			process = &Parse::processClientBodyTimeout;
			break ;
		case 1:
			process = &Parse::processClientLimitMaxBody;
			break ;
		case 2:
			process = &Parse::processDisableSymlink;
			break ;
		case 3:
			process = &Parse::processErrorPage;
			para_limit = 2;
			exact_para_limit = false;
			break ;
		case 4:
			process = &Parse::processEtag;
			break ;
		case 5:
			process = &Parse::processIfModifiedSince;
			break ;
		case 6:
			process = &Parse::processRoot;
			break ;
		case 7:
			process = &Parse::processIndex;
			exact_para_limit = false;
			break ;
		case 8:
			process = &Parse::processAutoIndex;
			break;
		case 9:
			process = &Parse::processAutoIndexExactSize;
			break;
		case 10:
			process = &Parse::processAutoIndexLocalTime;
			break;
		case 11:
			process = &Parse::processAccessLog;
			break;
		case 12:
			process = &Parse::processErrorLog;
			break;
		default:
			return (false);
	}
	directive_ptr = parameter[index];
	processDirective(process);
	return (true);
}

/**
 * @brief	check keyword in the server block the func will loop through all known
 * 			keyword and assign the proper function pointer to pass it into
 * 			processDirective to futher tokenize the arugment for the keyword and
 * 			store it into the ServerInfo
 * 
 * @param	keyw the keyword in the ServerInfo being process
 * 
 * @todo	testing listen, server_name, root, index for now
 * 			index can take multiple arug idk how that works
 */
void	Parse::processServer( const std::string &directive ) {
	static const char	*parameter[] = {
		"listen", "client_header_buffer_size", "client_header_timeout",
		"merge_slash", "server_name", "try_files", NULL
	};
	void (Parse::*process)(std::string &);
	process = NULL;
	if (processInfoBlock(directive)) {
		return ;
	}
	int	index = checkMatch(parameter, directive);
	switch (index)
	{
		case 0:
			process = &Parse::processListen;
			exact_para_limit = false;
			break ;
		case 1:
			process = &Parse::processClientHeaderBufferSize;
			break;
		case 2:
			process = &Parse::processClientHeaderTimeout;
			break ;
		case 3:
			process = &Parse::processMergeSlash;
			break;
		case 4:
			process = &Parse::processServerName;
			break;
		case 5:
			process = &Parse::processTryFiles;
			para_limit = 2;
			exact_para_limit = false;
			break;
		default:
			throw ParsingConfError(unknown_directive, directive.c_str());
	}
	directive_ptr = parameter[index];
	processDirective(process);
	if (index == 0) {
		std::string empty;
		processListen(empty); // special case
	}
}

void	Parse::processLocation( const std::string &directive ) {
	static const char	*parameter[] = {
		"alias", "internal", NULL
	};
	void (Parse::*process)(std::string &);
	process = NULL;
	if (processInfoBlock(directive)) {
		return ;
	}
	int index = checkMatch(parameter, directive);
	switch (index) {
		case 0:
			process = &Parse::processAlias;
			break;
		case 1:
			process = &Parse::processInternal;
			para_limit = 0;
			break;
		default:
			throw ParsingConfError(unknown_directive, directive.c_str());
	}
	directive_ptr = parameter[index];
	processDirective(process);
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
	else if (token == "}")
	{
		bracket_no --;
		block_level --;
		if (bracket_no < 0) {
			throw ParsingConfError(extra_delimitor, "}");
		}
		if (!bracket_no && !block_level) {
			server->addServerInfo(serverinfo);
			std::cout << "reset serverinfo\n";
			serverinfo.reset();
		}
		if (bracket_no == 1 && block_level == 1) {
			serverinfo.addLocation(location);
			std::cout << "reset location info\n";
			location.reset();
		}
		return ;
	}
	block_level = checkLevel(block_level, token);
	std::cout << "block level: " << block_level << ", token: " << token << '\n';
	para_limit = 1;
	exact_para_limit = true;
	if (block_level == 2 && bracket_no == 1) {
		location.addPath(token);
	}
	else if (bracket_no == 1 && block_level == 1) {
		ptr = &serverinfo;
		std::cout << "server picked up\n";
		processServer(token);
	}
	else if (bracket_no == 2 && block_level == 2) {
		ptr = &location;
		std::cout << "location picked up\n";
		if (!location.getLocationPath().length())
			throw ParsingConfError(invalid_no_parameter, "location");
		processLocation(token);
	}
	semicolon = false;
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
	ParsingConfError::line_no = &line_counter;
	ParsingConfError::type = "[emerg]";
	std::string	token;
	while (getNextLine()) {
		while (line_stream >> token) {
			processToken(token);
		}
	}
	if (bracket_no > 0) {
		throw ParsingConfError(excepted_delimitor, "}");
	}
}

/**
 * @brief	main entry function for parsing file
 * 			check file and convert its content into a single string
 * 			the comments part will be removed first
 * 			then the content will pass into a stringstream to be tokenized and processed
 * 			(filename is assumed to be set in construction)
 * 
 * @throws	ParsingFileError, basically all the options lulz
 * 
*/
void Parse::parseConfigFile( void ) {
	fname = filename.c_str();
	ParsingFileError::type = "[emerg]";
	if (!server) {
		throw ParsingFileError(-1);
	}
	CheckFile	check(fname);
	check.checking(F_OK | R_OK);
	if (!(!check.getAccessbility() && check.getType() == file))
		throw ParsingFileError(file_open);
	std::string	content;
	if (!check.getFileContent(content)) // statement terbalik, fixed
		throw ParsingFileError(file_open);
	if (!content.length())
		throw ParsingFileError(file_empty);
	removeComments(content);
	insertDelimWhiteSpace(content, "{};");
	content_stream.str(content);
	processContent();
	server->normalizeDefaultSetting();
}

boolean	Parse::processBoolParameter( const std::string &token, const char *directive ) {
	if (no_para > 1) {
		throw ParsingConfError(invalid_no_parameter, directive);
	}
	if (token == "on") {
		return (on);
	}
	if (token == "off") {
		return (off);
	}
	throw ParsingConfError(invalid_parameter, directive);
}

uint64_t	Parse::processSizeParameter( std::string &token, const char *directive ) {
	if (no_para > 0) {
		throw ParsingConfError(invalid_no_parameter, directive);
	}
	uint64_t	out;
	try {
		out = checkSize(token.begin(), token.end());
	}
	catch (const std::invalid_argument &e) {
		throw ParsingConfError(invalid_parameter, directive);
	}
	return (out);
}

uint64_t	Parse::processTimeParameter( std::string &token, const char *directive ) {
	if (no_para > 0) {
		throw ParsingConfError(invalid_no_parameter, directive);
	}
	uint64_t	out;
	try {
		out = checkTime(token.begin(), token.end());
	}
	catch (const std::invalid_argument &e) {
		throw ParsingConfError(invalid_parameter, directive);
	}
	return (out);
}

void	Parse::checkParameterEnd( void ) {
	if (exact_para_limit) {
		if (no_para != para_limit)
			throw ParsingConfError(invalid_no_parameter, directive_ptr);
	} else {
		if (no_para < para_limit)
			throw ParsingConfError(invalid_no_parameter, directive_ptr);
	}
}

void	Parse::processClientLimitMaxBody( std::string &token ){
	ptr->setClientMaxBodySize(processSizeParameter(token, directive_ptr));
}

void	Parse::processClientBodyTimeout( std::string &token ) {
	ptr->setClientBodyTimeout(processTimeParameter(token, directive_ptr));
}

void	Parse::processErrorPage( std::string &token ) {
	std::cout << "testing add " << no_para << "\n";
	if (no_para == 1) {
		ptr->addErrorPage();
	}
	try {
		ptr->addErrorPage(token);
	} catch ( const std::exception &e ) {
		throw ParsingConfError(invalid_parameter, directive_ptr);
	}
}

void	Parse::processEtag( std::string &token ) {
	ptr->setEtag(processBoolParameter(token, directive_ptr));
}

void	Parse::processIfModifiedSince( std::string &token ) {
	if (token == "off") {
		ptr->setModifySince(off_);
	} else if (token == "exact") {
		ptr->setModifySince(exact);
	} else if (token == "before") {
		ptr->setModifySince(before);
	} else {
		throw ParsingConfError(invalid_parameter, directive_ptr);
	}
}


void	Parse::processDisableSymlink( std::string &token ) {
	ptr->setCheckSymlinks(processBoolParameter(token, directive_ptr));
}

void	Parse::processRoot( std::string &token ) {
	ptr->addRoot(token);
}

void	Parse::processIndex( std::string &token ) {
	ptr->addIndex(token);
}

void	Parse::processAutoIndex( std::string &token ) {
	ptr->toggleAutoIndex(processBoolParameter(token, directive_ptr));
}

void	Parse::processAutoIndexExactSize( std::string &token ) {
	ptr->toggleAutoIndexSize(processBoolParameter(token, directive_ptr));
}

void	Parse::processAutoFormat( std::string &token ) {
	static const char *para[] = {
		"html", "xml", "json", "jsonp", NULL
	};
	int	check = checkMatch(para, token);
	if (check < 0)
		throw ParsingConfError(invalid_parameter, directive_ptr); 
	ptr->setAutoIndexFormat(check + 1);
}

void	Parse::processAutoIndexLocalTime( std::string &token ) {
	ptr->toggleAutoIndexTime(processBoolParameter(token, directive_ptr));	
}

void	Parse::processAccessLog( std::string &token ) {
	(void)token;
	// if (no_para == 1)
	// 	;
	// else if (no_para == 2)
	// 	;
	// else
	// 	;
}

void	Parse::processErrorLog( std::string &token ) {
	(void)token;
	// if (no_para == 1)
	// 	;
	// else if (no_para == 2)
	// 	;
	// else
	// 	;
}

void	Parse::processListenAddress( std::string &token ) {
	std::string	addr;
	std::string	port;
	size_t		check1, check2;

	if (all_of(token.begin(), token.end(), ::isdigit)) {
		port = token;
		addr = "::";
		goto addrinit;
	}
	check1 = addr.find('[');
	check2 = addr.find(']');
	if (check1 != std::string::npos || check2 != std::string::npos) {
		addr = token.substr(check1, check2);
		if (check2 != token.length() && token[check2 + 1] != ':')
			throw ParsingConfError(token, directive_ptr);
		port = token.substr(check2, token.length());
	} else {
		check2 = token.find(':');
		addr = token.substr(0, check2);
		if (check2 != std::string::npos) {
			port = token.substr(check2 + 1, token.length());
		}
	}
	if (addr == "*")
		addr = "::";
	if (!all_of(port.begin(), port.end(), ::isdigit)) {
		throw ParsingConfError(token, directive_ptr);
	}
	if (!port.length()) {
		port = "80";
	}
	addrinit:
	listen_socket.addAddress(addr, port);
	if (listen_socket.getStatus()) {
		throw ParsingConfError(token, directive_ptr);
	}
}

bool	Parse::processListenPara1( std::string &token ) {
	typedef void (ListenSocket::*setPara)( void );
	static const char *parameter[] = {
		"default_server", "ipv6only", "reuseport", NULL
	};
	static setPara	func[] = {
		&ListenSocket::setDefaultServer, &ListenSocket::setIpv6, &ListenSocket::setReusePort
	};
	int	i = -1;
	while (parameter[++ i]) {
		if (!token.compare(parameter[i])) {
			(listen_socket.*func[i])();
			return (true);
		}
	}
	return (false);
}

bool	Parse::processListenPara2( std::string &token, size_t pos ) {
	typedef void (ListenSocket::*setPara)( uint64_t );
	static const char *parameter[] = {
		"backlog", "rcvbuf", "sndbuf", NULL
	};
	static setPara	func[] = {
		&ListenSocket::setBackLog, &ListenSocket::setRcvBuf, &ListenSocket::setSndBuf
	};
	int i = -1;
	while (parameter[++ i]) {
		if (!token.compare(parameter[i])) {
			try {
				(listen_socket.*func[i])(checkSize(token.begin() + pos, token.end()));
			}
			catch (std::invalid_argument &e) {
				throw ParsingConfError(parameter[i], token); 
			}
			return (true);
		}
	}
	return (false);
}

void	Parse::processListenKeepAlive( std::string &token, size_t pos ) {
	if (!token.compare(pos + 1, token.length(), "off")) {
		return ;
	}
	if (!token.compare(pos + 1, token.length(), "on")) {
		listen_socket.setKeepalive();
		return ;
	}
	int		i = -1;
	long	val[3] = {-1, -1, -1};
	size_t	check = pos;
	while (++ i < 2) {
		size_t	delim = token.find(':', pos);
		if (delim == std::string::npos) {
			throw std::invalid_argument("delim not found");
		}
		val[i] = checkSize(token.begin() + check, token.begin() + delim);
		check += delim;
	}
	val[i] = checkSize(token.begin() + check, token.end());
	listen_socket.setKeepalive(val[0], val[1], val[2]);
}

void	Parse::processListen( std::string &token ) {
	static bool start = true;
	if (!token.length()) {
		start = true;
		serverinfo.addListen(listen_socket);
		listen_socket.reset();
		return ;
	}
	if (start) {
		start = false;
		processListenAddress(token);
	} else {
		if (processListenPara1(token))
			return ;
		size_t	pos = token.find('=');
		if (pos == std::string::npos) {
			throw ParsingConfError(invalid_parameter, directive_ptr);
		}
		if (processListenPara2(token, pos))
			return ;
		if (token.compare(0, pos, "so_keepalive")) {
			throw ParsingConfError(invalid_parameter, directive_ptr);	
		}
		try {
			processListenKeepAlive(token, pos);
		}
		catch (std::invalid_argument &e) {
			throw ParsingConfError("so_keepalive", token);
		}
	}
}

void	Parse::processServerName( std::string &token ) {
	serverinfo.addServerName(token);
}

void	Parse::processClientHeaderBufferSize( std::string &token ) {
	serverinfo.setClientHeaderBufferSize(processSizeParameter(token, directive_ptr));
}

void	Parse::processClientHeaderTimeout( std::string &token ) {
	serverinfo.setClientBodyTimeout(processTimeParameter(token, directive_ptr));
}

void	Parse::processMergeSlash( std::string &token ) {
	serverinfo.setMergeSlash(processBoolParameter(token, directive_ptr));
}

void	Parse::processAlias( std::string &token ) {
	location.addAlias(token);
}

void	Parse::processInternal( std::string &token ) {
	(void)token;
}

void	Parse::processTryFiles( std::string &token ) {
	ptr->addTryFiles(token);
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

ServerInfo	Parse::getServerInfo( void ) const {
	return (this->serverinfo);
}

// end of getters

Parse::ParsingFileError::ParsingFileError( int val ) { msgInit(val); }

Parse::ParsingFileError::~ParsingFileError( void ) throw() { }

void	Parse::ParsingFileError::msgInit( int val ) {
	msg = Parse::prog_name;
	msg += ": ";
	msg += type;
	msg += ": ";
	switch (val)
	{
	case file_extent:
		msg += "expected .conf extension";
		break ;
	case file_open:
		msg += "open()";
		break ;
	case file_empty:
		msg += "conf file is empty";
		break ;
	default:
		msg += "Unknown Error";
		break ;
	}
	msg += " \"";
	msg += Parse::fname;
	msg += "\" ";
	msg += "failed";
	if (val != file_open)
		return ;
	msg += " (";
	msg += to_String(errno) + ": ";
	msg += std::strerror(errno);
	msg += ')';
}

const char*	Parse::ParsingFileError::what() const throw() {
	return (msg.c_str());
}

Parse::ParsingConfError::ParsingConfError( int type, const char *directive ) {
	msgInit(type, directive);
}

Parse::ParsingConfError::ParsingConfError( const char *para, const std::string &token ) {
	msgInit(para, token);
}

Parse::ParsingConfError::ParsingConfError( const std::string &token, const char *directive ) {
	msgInit(token, directive);
}

Parse::ParsingConfError::~ParsingConfError( void ) throw() { }

void	Parse::ParsingConfError::msgInit( int type_, const char *directive ) {
	msg = Parse::prog_name;
	msg += ": ";
	msg += type;
	msg += ' ';
	switch (type_)
	{
	case unknown_directive:
		msg += "unknown directive \"";
		break ;
	case invalid_parameter:
		msg += "invalid parameter \"";
		break ;
	case invalid_no_parameter:
		msg += "invalid number of arguments in \"";
		break ;
	case excepted_delimitor:
		msg += "unexpected end of file, expecting \"";
		break ;
	case extra_delimitor:
		msg += "unexpected \"";
		break ;
	default:
		msg += "Unknown Error \"";
		break ;
	}
	msg += directive;
	msg += "\" in ";
	msg += Parse::fname;
	msg.append(":") += to_String(*line_no);
}

void	Parse::ParsingConfError::msgInit( const std::string &token, const char *directive ) {
	msg = Parse::prog_name;
	msg += ": ";
	msg += type;
	msg += " invalid host in \"";
	msg += token;
	msg += "\" of the ";
	msg.append("\"") += directive;
	msg += "\" directive in ";
	msg += Parse::fname;
	msg.append(":") += to_String(*line_no);	
}

void	Parse::ParsingConfError::msgInit( const char *parameter, const std::string &token ) {
	msg = Parse::prog_name;
	msg += ": ";
	msg += type;
	msg += " invalid ";
	msg += parameter;
	msg.append("\"") += token + '\"';
	msg += " in ";
	msg += Parse::fname;
	msg.append(":") += to_String(*line_no);	
}

const char*	Parse::ParsingConfError::what() const throw() {
	return (msg.c_str());
}

// void Parse::printLocations(const std::vector<Location*>& locations) {
// 	for (size_t i = 0; i < locations.size(); ++i) {
//         Location* loc = locations[i];
//         std::cout << "\nLocation " << i + 1 << ":\n";
//         std::cout << "Path: " << loc->path << "\n";

//         std::cout << "Root: ";
//         for (std::vector<std::string>::const_iterator it = loc->root.begin(); it != loc->root.end(); ++it) {
//             std::cout << *it << " ";
//         }
//         std::cout << "\n";

//         std::cout << "Autoindex: ";
//         for (std::vector<std::string>::const_iterator it = loc->autoindex.begin(); it != loc->autoindex.end(); ++it) {
//             std::cout << *it << " ";
//         }
//         std::cout << "\n";

//         std::cout << "Index: ";
//         for (std::vector<std::string>::const_iterator it = loc->index.begin(); it != loc->index.end(); ++it) {
//             std::cout << *it << " ";
//         }
//         std::cout << "\n";

//         std::cout << "Return Address: ";
//         for (std::vector<std::string>::const_iterator it = loc->return_add.begin(); it != loc->return_add.end(); ++it) {
//             std::cout << *it << " ";
//         }
//         std::cout << "\n";

//         std::cout << "Alias: ";
//         for (std::vector<std::string>::const_iterator it = loc->alias.begin(); it != loc->alias.end(); ++it) {
//             std::cout << *it << " ";
//         }
//         std::cout << "\n";

//         std::cout << "Allow Methods: ";
//         for (std::vector<std::string>::const_iterator it = loc->allow_methods.begin(); it != loc->allow_methods.end(); ++it) {
//             std::cout << *it << " ";
//         }
//         std::cout << "\n";

//         std::cout << "CGI Paths: ";
//         for (std::vector<std::string>::const_iterator it = loc->cgi_path.begin(); it != loc->cgi_path.end(); ++it) {
//             std::cout << *it << " ";
//         }
//         std::cout << "\n";

//         std::cout << "CGI Extensions: ";
//         for (std::vector<std::string>::const_iterator it = loc->cgi_ext.begin(); it != loc->cgi_ext.end(); ++it) {
//             std::cout << *it << " ";
//         }
//         std::cout << "\n\n";
//     }
// }
