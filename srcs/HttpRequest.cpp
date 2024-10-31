/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:16:03 by joshua            #+#    #+#             */
/*   Updated: 2024/10/31 11:16:25 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest( void ) :
Http(),
header(""),
body(""),
header_fields(),
valid_header(false),
method(),
uri(""),
protocol(""),
has_body(false),
content_type(TEXT),
content_length(0),
finished_request(false)
{ }

HttpRequest::HttpRequest( const HttpRequest &src ) : Http(src) {
	*this = src;
}

HttpRequest&	HttpRequest::operator=( const HttpRequest &src ) {
	if (this != &src) {
		header = src.header;
		body = src.body;
		header_fields = src.header_fields;
		valid_header = src.valid_header;
		method = src.method;
		uri = src.uri;
		protocol = src.protocol;
		has_body = src.has_body;
		content_type = src.content_type;
		content_length = src.content_length;
		finished_request = src.finished_request;
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
	return (0);
}

size_t	HttpRequest::addRequest( const std::string &str ) {
	if (has_body) {
		return (addBody(str, 0));
	}
	size_t pos = str.find("\r\n\r\n");
	if (pos == std::string::npos) {
		header += str;
		return (0);
	}
	pos += 4;
	header.append(str, 0, pos);
	validateHeader();
	if (method == POST || method == PUT) {
		if (!(validateBody()))
			return (str.length() - pos);
		return (addBody(str, pos));
	}
	else
		finished_request = true;
	return (str.length() - pos);
}

// use for header
static bool	normalizeString( std::string &str ) {
	static int diff = 'a' - 'A';
	static const char *invalid_char = "()<>@,;:\'\"/[]?={} ~";
	for (size_t i = 0; i < str.length(); i ++) {
		if ((::isupper(str[i]))) {
			str[i] += diff;
			continue ;
		}
		for (size_t j = 0; invalid_char[j]; j ++) {
			if (str[i] < 32 || str[i] == 127)
				return (false);
			if (str[i] == invalid_char[j])
				return (false);
		}
	}
	return (true);
}

static bool	checkValue( const std::string &str ) {
	for (size_t i = 0; i < str.length(); i ++) {
		if (str[i] < 32 || str[i] == 127)
			return (false);
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
	protocol = start.substr(j, i - j); // need validation
	// std:", " << start[i - 1] :cout << "proto ? " << protocol << '\n';
	// std::cout << "test i " << static_cast<int>(start[i]) << ", " << static_cast<int>(start[i - 1]) << '\n';
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
bool    HttpRequest::validateHeaderHelper( void ) {
	std::istringstream  ss(header);
	std::string         token;
	std::getline(ss, token);
	if (!(validateStartLine(token))) {
		std::cout << "Http start Error\n";
		return (false);
	}
	while (std::getline(ss, token)) {
		std::string buffer_field;
		std::string buffer_value;
		if (token[0] == '\r') {
			break ;
		}
		size_t	pos = token.find(":");
		if (pos == std::string::npos) {
			return (false);
		}
		buffer_field = token.substr(0, pos);
		if (normalizeString(buffer_field)) {
			return (false);
		}
		int space = (token[pos + 1] == ' ') ? 1 : 0;
		buffer_value = token.substr(pos + 1 + space);
		if (checkValue(buffer_value)) {
			return (false);
		}
		if (token[token.length() - 1] != '\r') {
			return (false);
		}
		token.erase(-- token.end());
		if (!addHeaderFields(buffer_field, buffer_value)) {
			return (false);
		}
	}
	token = ss.str();
	if (token != "\r\n")
		return (false);
	return (true);
}

bool	HttpRequest::validateHeader( void ) {
	valid_header = validateHeaderHelper();
	if (!valid_header)
		std::cout << "Http Header Error\n";
	return (valid_header);
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
	valid_header = true;
	return (true);
	fail:
		valid_header = false;
		return (false);
}

void	HttpRequest::normalizeUri( void ) {
	size_t pos = 0;
	while ((pos = uri.find("//", pos)) != std::string::npos) {
		uri.replace(pos, 2, "/");
	}
}

bool	HttpRequest::getValidHeader( void ) const { return (valid_header); }

bool	HttpRequest::isReady( void ) const { return (finished_request); }

const std::string&	HttpRequest::getUri( void ) const  { return (uri); }

const std::string&	HttpRequest::getHeaderStr( void ) const { return (header); }

std::ostream&	operator<<( std::ostream &o, const HttpRequest &req ) {
	o << "Request is" << ((req.isReady()) ? " " : " not ") << "ready\n";
	o << "Validate Header: " << ((req.getValidHeader()) ? "turw" : "false") << "\n";
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
