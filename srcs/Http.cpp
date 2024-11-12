/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 01:46:54 by joshua            #+#    #+#             */
/*   Updated: 2024/11/12 22:01:27 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

const Http::t_pairs	Http::pairing[] = {
	{100, "Continue"}, {101, "Switching Protocols"}, {102, "Processing"}, {103, "Early Hints"},
	{200, "OK"}, {201, "Created"}, {202, "Accepted"}, {203, "Non-Authoritative Information"},
	{204, "No Content"}, {205, "Reset Content"}, {206, "Partial Content"}, {207, "Multi-Status"},
	{208, "Already Reported"}, {226, "IM Used"},
	{300, "Multiple Choices"}, {301, "Moved Permanently"}, {302, "Found"}, {303, "See Other"},
	{304, "Not Modified"}, {305, "Use Proxy"}, {306, "Switch Proxy"}, {307, "Temporary Redirect"},
	{308, "Permanent Redirect"},
	{400, "Bad Request"}, {401, "Unauthorized"}, {402, "Payment Required"}, {403, "Forbidden"},
	{404, "Not Found"}, {405, "Method Not Allowed"}, {406, "Not Acceptable"},
	{407, "Proxy Authentication Required"}, {408, "Request Timeout"}, {409, "Conflict"}, {410, "Gone"},
	{411, "Length Required"}, {412, "Precondition Failed"}, {413, "Payload Too Large"},
	{414, "URI Too Long"}, {415, "Unsupported Media Type"}, {416, "Range Not Satisfiable"},
	{417, "Expectation Failed"}, {418, "I'm a teapot"}, {421, "Misdirected Request"},
	{422, "Unprocessable Content"}, {423, "Locked"}, {424, "Failed Dependency"}, {425, "Too Early"},
	{426, "Upgrade Required"}, {428, "Precondition Required"}, {429, "Too Many Requests"},
	{431, "Request Header Fields Too Large"}, {451, "Unavailable For Legal Reasons"},
	{500, "Internal Server Error"}, {501, "Not Implemented"}, {502, "Bad Gateway"},
	{503, "Service Unavailable"}, {504, "Gateway Timeout"}, {505, "HTTP Version Not Supported"},
	{506, "Variant Also Negotiates"}, {507, "Insufficient Storage"}, {508, "Loop Detected"},
	{510, "Not Extended"}, {511, "Network Authentication Required"},
	{999, NULL}
};

const char *Http::methods[] = {
	"GET", "POST", "PUT", "DELETE", NULL
};

const char *Http::fields[] = {
	"host", "user-agent", "accept", "accept-language", "accept-encoding",
	"connection", "referer", "cookie", "cache-control", "content-type",
	"content-length", NULL
};

const Http::t_types Http::mime_types[] = {
	{"aac", "audio/aac"}, 
	{"abw", "application/x-abiword"}, 
	{"apng", "image/apng"},
	{"avi", "video/x-msvideo"},
	{"bmp", "image/bmp"},
	{"css", "text/css"},
	{"csv", "text/csv"},
	{"doc", "application/msword"},
	{"gif", "image/gif"},
	{"html", "text/html"},
	{"htm", "text/html"},
	{"ico", "image/vnd.microsoft.icon"},
	{"jpeg", "image/jpeg"},
	{"jpg", "image/jpeg"},
	{"js", "text/javascript"},
	{"json", "application/json"},
	{"mp3", "audio/mpeg"},
	{"mp4", "video/mp4"},
	{"pdf", "application/pdf"},
	{"png", "image/png"},
	{"svg", "image/svg+xml"},
	{"txt", "text/plain"},
	{"xml", "application/xml"},
	// Add more MIME types as needed
	{NULL, NULL}
};

Http::Http( void ) :
ready(false),
header(""), body(""), combine(""), bytes_sent(0),
content_length(0), content_type(""), header_fields() { }

Http::Http( const Http &src ) { *this = src; }

Http::~Http( void ) { }

Http&	Http::operator=( const Http &src ) {
	if (this == &src) {
		return (*this);
	}
	ready = src.ready;
	header = src.header;
	body = src.body;
	combine = src.combine;
	bytes_sent = src.bytes_sent;
	content_length = src.content_length;
	content_type = src.content_type;
	header_fields = src.header_fields;
	return (*this);
}

// use for header
bool	Http::validateField( std::string &str ) const {
	static int diff = 'a' - 'A';
	static const char *invalid_char = "()<>@,;:\'\"/[]?={} ~";
	// std::cout << "test Http Request Valid test str in checkField: " << str << "\n";
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

bool	Http::validateValue( const std::string &str ) const {
	// std::cout << "checkValue " << str << '\n';
	for (size_t i = 0; i < str.length() - 1; i ++) {
		if (str[i] < 32 || str[i] == 127) {
			// std::cout << "invalid_char found (" << str[i] << ")[" << static_cast<int>(str[i]) << "]\n";
			return (false);
		}
	}
	return (true);
}

bool	Http::addHeaderFields( const std::string &field, const std::string &val ) {
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

std::string&	Http::modifyHeader( void ) { return (header); }

std::string&	Http::modifyBody( void ) { return (body); }

void	Http::finishHttp( void ) { combine = header + body; ready = true; }

size_t	Http::getHeaderLength( void ) const { return (header.length()); }

size_t	Http::getBodyLength( void ) const { return (body.length()); }

size_t	Http::getTotalLength( void ) const { return (combine.length()); }

size_t	Http::getRemainderHeader( void ) const { return (header.length() - bytes_sent); }

size_t	Http::getRemainderBody( void ) const { return (body.length() - bytes_sent); }

size_t	Http::getRemainderHttp( void ) const { return (combine.length() - bytes_sent); }

const char*	Http::getPtr2Header( void ) const { return (header.c_str() + bytes_sent); }

const char*	Http::getPtr2Body( void ) const { return (body.c_str() + bytes_sent); }

const char*	Http::getPtr2Http( void ) const { return (combine.c_str() + bytes_sent); }

const std::string&	Http::getHeader( void ) const { return (header); }

const std::string&	Http::getBody( void ) const { return (body); }

const std::string&	Http::getField( const char *str ) const {
	typedef std::map<std::string, std::string>::const_iterator	iter;
	static const std::string	empty;
	if (!str) {
		return (empty);
	}
	iter it = header_fields.find(str);
	if (it == header_fields.end()) {
		return (empty);
	}
	return (it->second);
}

bool	Http::isReady( void ) const { return (ready); }

bool	Http::checkSend( size_t bytes, size_t len ) {
	bytes_sent += bytes;
	if (bytes_sent >= len) {
		return (true);
	}
	return (false);
}

bool	Http::checkSendHeader( size_t bytes ) {
	return (checkSend(bytes, header.length()));
}

bool	Http::checkSendBody( size_t bytes ) {
	return (checkSend(bytes, body.length()));
}

bool	Http::checkSendHttp( size_t bytes ) {
	return (checkSend(bytes, combine.length()));
}

void	Http::reset( void ) {
	ready = false;
	header.clear();
	body.clear();
	combine.clear();
	content_length = 0;
	content_type = "text/plain";
	bytes_sent = 0;
}

const char	*Http::fetchMsg( int status ) {
	for (size_t i = 0; pairing[i].msg; i ++) {
		if (status == pairing[i].status) {
			return (pairing[i].msg);
		}
	}
	return (NULL);
}

const char	*Http::getMimeType( const std::string &ext ) {
	for (size_t i = 0; mime_types[i].extension; i ++) {
		if (ext == mime_types[i].extension) {
			return (mime_types[i].mime_type);
		}
	}
	return ("application/octet-stream");
}

int	Http::checkMethods( const std::string &str ) {
	return (iterEnumStrcmp(GET, DELETE, str, methods));
}

int	Http::checkField( const std::string &str ) {
	return (iterEnumStrcmp(HOST, C_LEN, str, fields));
}

bool	Http::makeReponseHeader( std::string &str, int status ) const {
	std::string	buffer = to_String(status) + ' ';
	for (size_t i = 0; pairing[i].msg; i ++) {
		if (status == pairing[i].status) {
			str = buffer + pairing[i].msg;
			return (true);
		}
	}
	return (false);
}
