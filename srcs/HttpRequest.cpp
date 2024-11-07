/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:16:03 by joshua            #+#    #+#             */
/*   Updated: 2024/11/07 18:52:47 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "EmbeddedVariable.hpp"

HttpRequest::HttpRequest( void ) :
Http(),
header_fields(),
error(),
method(),
url(""),
uri(""),
path(""),
query(""),
protocol(""),
has_body(false)
{ }

HttpRequest::HttpRequest( const HttpRequest &src ) : Http(src) {
	*this = src;
}

HttpRequest&	HttpRequest::operator=( const HttpRequest &src ) {
	if (this != &src) {
		Http::operator=(src);
		header_fields = src.header_fields;
		error = src.error;
		method = src.method;
		url = src.url;
		uri = src.uri;
		path = src.path;
		query = src.query;
		protocol = src.protocol;
		has_body = src.has_body;
	}
	return (*this);
}

HttpRequest::~HttpRequest( void ) { }

size_t	HttpRequest::addBody( const std::string &str, size_t pos ) {
	size_t excepted_bytes = content_length - body.length();
	if (str.length() - pos > excepted_bytes)
	{
		body.append(str, pos, excepted_bytes);
		return (str.length() - pos - excepted_bytes);
	}
	body.append(str, pos);
	ready = true;
	return (0);
}

size_t	HttpRequest::addRequest( const std::string &str ) {
	if (has_body) {
		return (addBody(str, 0));
	}
	size_t pos = str.find("\r\n\r\n");
	if (pos == std::string::npos) {
		header += str;
		if (header.length() > header_limit) {
			error = limit_exceed;
		}
		return (0);
	}
	pos += 4;
	header.append(str, 0, pos);
	if (header.length() > header_limit) {
		error = limit_exceed;
		return (str.length() - pos);
	}
	validateHeader();
	if (method == POST || method == PUT) {
		if (!(validateBody()))
			return (str.length() - pos);
		return (addBody(str, pos));
	}
	else
		ready = true;
	return (str.length() - pos);
}

// use for header
static bool	normalizeString( std::string &str ) {
	static int diff = 'a' - 'A';
	static const char *invalid_char = "()<>@,;:\'\"/[]?={} ~";
	// std::cout << "test Http Request Valid test str in normalizeString: " << str << "\n";
	for (size_t i = 0; i < str.length(); i ++) {
		if ((::isupper(str[i]))) {
			str[i] += diff;
			continue ;
		}
		for (size_t j = 0; invalid_char[j]; j ++) {
			if (str[i] < 32 || str[i] == 127 || str[i] == invalid_char[j]) {
				// std::cout << "invalid_char found (" << str[i] << ")[" << static_cast<int>(str[i]) << "]\n";
				return (false);
			}
		}
	}
	return (true);
}

static bool	checkValue( const std::string &str ) {
	// std::cout << "checkValue " << str << '\n';
	for (size_t i = 0; i < str.length() - 1; i ++) {
		if (str[i] < 32 || str[i] == 127) {
			// std::cout << "invalid_char found (" << str[i] << ")[" << static_cast<int>(str[i]) << "]\n";
			return (false);
		}
	}
	return (true);
}

bool	HttpRequest::validateUrl( void ) {
	if (uri[0] != '/') {
		return (false);
	}
	if (!(EmbeddedVariable::checkUrl(uri))) {
		return (false);
	}
	std::string	buffer;
	url = "http://";
	buffer = getField("host");
	if (!buffer.length()) {
		return (false);
	}
	url += buffer;
	size_t pos = uri.find('?');
	path = uri.substr(0, pos);
	if (pos != std::string::npos) {
		query = uri.substr(pos + 1);
	}
	return (true);
}

bool	HttpRequest::validateStartLine( const std::string &start ) {
	std::string buffer;
	size_t i = 0, j = 0;
	for (; i < start.length(); i ++) {
		if (start[i] == ' ')
			break ;
		if (!(::isupper(start[i])))
			return (false);
	}
	if (!i)
		return (false);
	buffer = start.substr(0, i);
	// std::cout << "test error\n";
	int check_method = checkMethods(buffer);
	if (check_method < 0)
		return (false);
	// std::cout << "test error 2\n";
	method = static_cast<http_method>(check_method);
	i ++;
	j = i;
	for (; i < start.length(); i ++) {
		if (start[i] == ' ')
			break ;
	}
	if (j == i)
		return (false);
	uri = start.substr(j, i - j);
	i ++;
	j = i;
	// std::cout << "test error 3\n";
	for (; i < start.length(); i ++) {
		if (start[i] == ' ' || start[i] == '\r')
			break ;
	}
	if (j == i)
		return (false);
	protocol = start.substr(j, i - j);
	if (protocol != "HTTP/1.1") {
		return (false);
	}
	if (start[i] != '\r' || i != start.length() - 1) {
		return (false);
	}
	return (true);
}

bool	HttpRequest::addHeaderFields( const std::string &field, const std::string &val ) {
	typedef std::map<std::string, std::string>::iterator iter;
	iter it = header_fields.find(field);
	if (it == header_fields.end()) {
		header_fields[field] = val;
		return (true);
	}
	if (field == fields[COOKIE] || field == fields[CACHE]) {
		it->second += "; ";
		it->second += val;
		return (true);
	}
	return (false);
}

// checkHeader field and val size
bool	HttpRequest::validateHeader( void ) {
	std::istringstream	ss(header);
	std::string			token;
	bool				check = false;
	error = malform_header;
	std::getline(ss, token);
	if (!(validateStartLine(token))) {
		std::cout << "Http start Error\n";
		return (false);
	}
	while (std::getline(ss, token)) {
		std::string buffer_field;
		std::string buffer_value;
		// std::cout << token << '\n';
		if (token == "\r") {
			check = true;
			break ;
		}
		size_t	pos = token.find(":");
		if (pos == std::string::npos) {
			std::cout << "test Http Request Valid cant find :\n";
			return (false);
		}
		buffer_field = token.substr(0, pos);
		if (!(normalizeString(buffer_field))) {
			std::cout << "test Http Request Valid have invalid char\n";
			return (false);
		}
		int space = (token[pos + 1] == ' ') ? 1 : 0;
		buffer_value = token.substr(pos + 1 + space);
		if (!(checkValue(buffer_value))) {
			std::cout << "test Http Request Valid Value have invalid char\n";
			return (false);
		}
		if (token[token.length() - 1] != '\r') {
			std::cout << "test Http Request Valid doesnt end with \\r \n";
			return (false);
		}
		token.erase(-- token.end());
		if (!addHeaderFields(buffer_field, buffer_value)) {
			std::cout << "test Http Request Valid have dup or invalid header value\n";
			return (false);
		}
	}
	if (!check) {
		std::cout << "test Http Request Valid doesnt end with \\r\\n\n";
		return (false);
	}
	if (!validateUrl()) {
		return (false);
	}
	error = 0;
	return (true);
}

bool	HttpRequest::validateBody( void ) {
	typedef std::map<std::string, std::string>::iterator iter;
	iter it = header_fields.find(fields[C_TYPE]);
	int check = -1;
	if (it == header_fields.end()) {
		goto fail;
	}
	if (!getMimeType(it->second)) {
		goto fail;
	}
	content_type = static_cast<type>(check);
	it = header_fields.find(fields[C_LEN]);
	if (it == header_fields.end()) {
		goto fail;
	}
	if (!(all_of(it->second.begin(),
		it->second.end(), ::isdigit
	))) {
		goto fail;
	}
	content_length =
	static_cast<uint64_t> (
		std::atoll(it->second.c_str())
	);
	return (true);
	fail:
		return (false);
}

void	HttpRequest::normalizeUri( void ) {
	size_t pos = 0;
	while ((pos = uri.find("//", pos)) != std::string::npos) {
		uri.replace(pos, 2, "/");
	}
}

int	HttpRequest::getValidHeader( void ) const {
	if (error & malform_header) {
		return (400);
	}
	if (error & limit_exceed) {
		return (431);
	}
	return (0);
}

const std::string&	HttpRequest::getField( const char *field ) const {
	typedef std::map<std::string, std::string>::const_iterator	iter;
	static std::string	empty;
	iter it = header_fields.find(field);
	if (it == header_fields.end()) {
		return (empty);
	}
	return (it->second);
}

const std::string&	HttpRequest::getUrl( void ) const { return (url); }

const std::string&	HttpRequest::getUri( void ) const { return (uri); }

const std::string&	HttpRequest::getPath( void ) const { return (path); }

const std::string&	HttpRequest::getQuery( void ) const { return (query); }

const std::string&	HttpRequest::getHeaderStr( void ) const { return (header); }

const std::string&	HttpRequest::getProtocol( void ) const { return (protocol); }

std::string	HttpRequest::getContentType( void ) const {
	return (std::string(methods[static_cast<int>(method)]));
}

uint64_t	HttpRequest::getContentLength( void ) const { return (content_length); }

std::ostream&	operator<<( std::ostream &o, const HttpRequest &req ) {
	o << "Request is" << ((req.isReady()) ? " " : " not ") << "ready\n";
	o << "Validate Header: " << ((req.getValidHeader()) ? "true" : "false") << "\n";
	o << "Header\n" << req.getHeaderStr();
		// 	http_method	getMethod( void ) const;
		// const std::string&	getUri( void ) const;
		// const std::string&	getProtocol( void ) const; // empty
		// bool		getHasBody( void ) const;
		// bool		getValidHeader( void ) const;
		// bool		isReady( void ) const; //empty

		// type		getContentType( void ) const;
		// uint64_t	getContentLength( void ) const;
	return (o);
}
