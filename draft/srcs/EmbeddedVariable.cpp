/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EmbeddedVariable.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 04:31:35 by joshua            #+#    #+#             */
/*   Updated: 2024/09/27 15:25:10 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EmbeddedVariable.hpp"

/*
	arg_name,
	args,
	query_string,
	content_length,
	content_type,
	document_root,
	document_url,
	host,
	url,
	remote_addr,
	remote_port,
	request_method,
	scheme,
	status
*/

const char	**EmbeddedVariable::variables = (char *[]) {
	"arg_name", "args", "query_string", "content_length", "content_type",
	"document_root", "document_url", "host", "url",
	"remote_addr", "remote_port", "request_method",
	"scheme", "status", NULL
};

const uint8_t	*EmbeddedVariable::shortform = (uint8_t []) {
	arg_name, args, query_string, content_length, content_type, document_root,
	document_url, host, url, remote_addr, remote_port, request_method,
	scheme, status
};

static size_t   find_set( std::string &str, size_t pos ) {
	static char *special = "/$";
	while (pos < str.length()) {
		size_t iter = -1;
		while (special[++ iter]) {
			if (str[pos] == special[iter])
				return (pos);
		}
		pos ++;
	}
	return (pos);
}

EmbeddedVariable::~EmbeddedVariable( void ) { }

void	EmbeddedVariable::shortFormString( std::string &str ) {
	size_t  start_dollar = str.find('$'), end = 0;
	while (start_dollar != std::string::npos) {
		end = find_set(str, start_dollar);
		for (size_t iter = 0; variables[iter]; iter ++) {
			if (!str.compare(start_dollar + 1, end - start_dollar - 1, variables[iter])) {
				std::string ref;
				ref += static_cast<char>(shortform[iter]);
				str.replace(start_dollar, end - start_dollar, ref);
			}
		}
		start_dollar = str.find('$', end);
	}
}

void	EmbeddedVariable::resolveString( std::string &str, ... ) {
	
}

// void	EmbeddedVariable::shortFormStringAllow( std::string &str, int8_t *allow ) {
// 	size_t  start_dollar = str.find('$'), end = 0;
// 	while (start_dollar != std::string::npos) {
// 		end = find_set(str, start_dollar);
// 		for (size_t iter = 0; variables[iter]; iter ++) {
// 			if (!str.compare(start_dollar + 1, end - start_dollar - 1, variables[iter])) {
// 				std::string ref;
// 				ref += static_cast<char>(shortform[iter]);
// 				str.replace(start_dollar, end - start_dollar, ref);
// 			}
// 		}
// 		start_dollar = str.find('$', end);
// 	}
// }

// void	shortFormStringDeny( std::string &str, int8_t *deny ) {
	
// }

EmbeddedVariable::EmbeddedVariable( void ) { }

EmbeddedVariable::EmbeddedVariable( const EmbeddedVariable &src ) { (void)src; }

EmbeddedVariable&	EmbeddedVariable::operator=( const EmbeddedVariable &src )
{ (void)src; return (*this); }