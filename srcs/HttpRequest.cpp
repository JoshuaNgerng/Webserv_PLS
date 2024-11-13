/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:16:03 by joshua            #+#    #+#             */
/*   Updated: 2024/11/14 00:45:42 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "EmbeddedVariable.hpp"

HttpRequest::HttpRequest( void ) :
Http(),
header_limit(1000),
error(malform_header),
method(),
url(""),
uri(""),
path(""),
query(""),
protocol(""),
has_body(false)
{ }

HttpRequest::HttpRequest( size_t header_size ) :
Http(),
header_limit(header_size),
error(malform_header),
method(),
url(""),
uri(""),
path(""),
query(""),
protocol(""),
has_body(false)
{ }

HttpRequest::HttpRequest( const HttpRequest &src ) : Http() {
	*this = src;
}

HttpRequest&	HttpRequest::operator=( const HttpRequest &src ) {
	if (this != &src) {
		Http::operator=(src);
		header_limit = src.header_limit;
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
	// std::cerr << "added Body pos " <<  pos << ", buffer len " << str.length() <<"\n";
	size_t excepted_bytes = content_length - body.length();
	if (str.length() - pos > excepted_bytes)
	{
		// std::cerr << "add Body excced excepted\n";
		body.append(str, pos, excepted_bytes);
		ready = true;
		return (str.length() - pos - excepted_bytes);
	}
	body.append(str, pos);
	if (body.length() != content_length) {
		// std::cerr << "body still needed, " << body.length() << " expected: "
		// << content_length << '\n';
		return (0);
	}
	// std::cerr << "add all in body ready to go\n";
	ready = true;
	return (0);
}

size_t	HttpRequest::addRequest( const std::string &str ) {
	if (has_body) {
		return (addBody(str, 0));
	}
	size_t pos = str.find("\r\n\r\n");
	// std::cout << "str: " << str << '\n';
	// std::cout << "test \\r\\n\\r\\n: " << pos << '\n';
	if (pos == std::string::npos) {
		header += str;
		if (header.length() > header_limit) {
			error = limit_exceed;
		}
		return (0);
	}
	pos += 4;
	header.append(str, 0, pos);
	// std::cout << "wait what\n" << header;
	if (header.length() > header_limit) {
		error = limit_exceed;
		// std::cout << "limit excced error\n";
		return (str.length() - pos);
	}
	validateHeader();
	header_ready = true;
	if (method == POST || method == PUT) {
		// std::cout << "body method called\n";
		if (!(validateBody()))
			return (str.length() - pos);
		has_body = true;
		// std::cerr << "start addBody, " << pos << '\n';
		return (addBody(str, pos));
	}
	else
		ready = true;
	return (str.length() - pos);
}

bool	HttpRequest::validateUrl( void ) {
	// std::cout << "huh?\n";
	if (uri[0] != '/') {
		return (false);
	}
	if (!(EmbeddedVariable::checkUrl(uri))) {
		return (false);
	}
	// std::cout << "validate here?\n";
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
	path = EmbeddedVariable::decodeUrl(path);
	// std::cout << "decode path " << path << '\n';
	query = EmbeddedVariable::decodeUrl(query);
	// std::cout << "decode query " << query << '\n';
	return (true);
}

bool	HttpRequest::validateStartLine( const std::string &start ) {
	// std::cout << "Header Start line " << start << '\n';
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
	if (check_method < 0) {
		// std::cout << "error http method " << buffer << "\n";
		return (false);
	}
	// std::cout << "test error 2\n";
	method = static_cast<http_method>(check_method);
	// std::cout << "valid method " << buffer << " " << method << '\n';
	i ++;
	j = i;
	for (; i < start.length(); i ++) {
		if (start[i] == ' ')
			break ;
	}
	if (j == i)
		return (false);
	uri = start.substr(j, i - j);
	// std::cout << "uri called " << uri << "\n";
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
	// std::cout << "validate here?\n" ;
	std::getline(ss, token);
	if (!(validateStartLine(token))) {
		// std::cout << "Http start Error\n";
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
		if (!(validateField(buffer_field))) {
			std::cout << "test Http Request Valid have invalid char\n";
			return (false);
		}
		int space = (token[pos + 1] == ' ') ? 1 : 0;
		buffer_value = token.substr(pos + 1 + space);
		if (!(validateValue(buffer_value))) {
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
	if (it == header_fields.end()) {
		std::cout << "no content-type\n";
		return (false);
	}
	content_type = it->second;
	it = header_fields.find(fields[C_LEN]);
	if (it == header_fields.end()) {
		std::cout << "no content-len\n";
		return (false);
	}
	size_t counter = 0;
	std::string::iterator ptr , ptr2;
	ptr = it->second.begin(), ptr2 = it->second.end();
	while (ptr != ptr2 && std::isspace(*ptr)) {
		counter ++;
		ptr ++;
	}
	while (ptr2 != ptr && std::isspace(*(ptr2 - 1))) {
		ptr2 --;
	}
	// std::cout << "testing |" << it->second << "|\n";
	if (!(all_of(ptr, ptr2, ::isdigit))) {
		std::cout << "invalid content-len\n";
		return (false);
	}
	content_length = static_cast<uint64_t>(
		std::atoll(it->second.c_str() + counter)
	);
	return (true);
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

Http::http_method	HttpRequest::getMethod( void ) const { return (method); }

const std::string&	HttpRequest::getUrl( void ) const { return (url); }

const std::string&	HttpRequest::getUri( void ) const { return (uri); }

const std::string&	HttpRequest::getPath( void ) const { return (path); }

const std::string&	HttpRequest::getQuery( void ) const { return (query); }

const std::string&	HttpRequest::getProtocol( void ) const { return (protocol); }

const std::string&	HttpRequest::getContentType( void ) const {
	return (content_type);
}

uint64_t	HttpRequest::getContentLength( void ) const { return (content_length); }

std::ostream&	operator<<( std::ostream &o, const HttpRequest &req ) {
	o << "Request is" << ((req.isReady()) ? " " : " not ") << "ready\n";
	o << "Validate Header: " << ((!req.getValidHeader()) ? "true" : "false") << "\n";
	o << "Header\n" << req.getHeader();
	o << "Body\n" << req.getBody();
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
