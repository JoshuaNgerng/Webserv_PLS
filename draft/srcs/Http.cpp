/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 01:46:54 by joshua            #+#    #+#             */
/*   Updated: 2024/09/30 23:32:16 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

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

Http::Http( const Http &src ) { (void)src; }

Http::~Http( void ) { }

int	Http::checkMethods( const std::string &str ) const {
	return (iterEnumStrcmp(GET, DELETE, str, methods));
}

int	Http::checkField( const std::string &str ) const {
	return (iterEnumStrcmp(HOST, C_LEN, str, fields));
}

int	Http::checkType( const std::string &str ) const {
	return (iterEnumStrcmp(TEXT, HTML, str, methods));
}
