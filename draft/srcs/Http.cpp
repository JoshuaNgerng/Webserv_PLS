/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 01:46:54 by joshua            #+#    #+#             */
/*   Updated: 2024/10/02 01:42:11 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Http.hpp"

const Http::t_pairs	*Http::pairing = (t_pairs []) {
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

const char *const *Http::methods = (char *[]) {
	"GET", "POST", "PUT", "DELETE", NULL
};

const char *const *Http::fields = (char *[]) {
	"host", "user-agent", "accept", "accept-language", "accept-encoding",
	"connection", "referer", "cookie", "cache-control", "content-type",
	"content-length", NULL
};

const char *const *Http::types = (char *[]) {
	"text/plain", "text/html", "application/octet-stream", NULL
};

Http::Http( void ) { }

Http::Http( const Http &src ) {
	*this = src;
}

Http&	Http::operator=( const Http &src ) {
	if (this != &src) {
		(void)src;
	}
	return (*this);
}

Http::~Http( void ) { }

const char	*Http::fetchMsg( int status ) {
	for (size_t i = 0; pairing[i].msg; i ++) {
		if (status == pairing[i].status) {
			return (pairing[i].msg);
		}
	}
	return (NULL);
}

int	Http::checkMethods( const std::string &str ) const {
	return (iterEnumStrcmp(GET, DELETE, str, methods));
}

int	Http::checkField( const std::string &str ) const {
	return (iterEnumStrcmp(HOST, C_LEN, str, fields));
}

int	Http::checkType( const std::string &str ) const {
	return (iterEnumStrcmp(TEXT, HTML, str, methods));
}
