/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EmbeddedVariable.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 04:31:35 by joshua            #+#    #+#             */
/*   Updated: 2024/11/07 10:11:16 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EmbeddedVariable.hpp"
#include "Client.hpp"

const char	*EmbeddedVariable::variables[] = {
	"arg_", "args", "query_string", "content_length", "content_type",
	"document_root", "document_url", "host", "url",
	"remote_addr", "remote_port", "request_method",
	"scheme", "status", NULL
};

const uint8_t	EmbeddedVariable::shortform[] = {
	arg_, args, query_string, content_length, content_type,
	document_root, document_uri, document_url, host, uri, url,
	remote_addr, remote_port, request_method, scheme, status
};

static size_t   find_set( std::string &str, size_t pos ) {
	static const char *special = "/$";
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

void	EmbeddedVariable::resolveString( std::string &str, const std::string &ref, const Client &client ) {
	str.reserve(ref.length());
	for (size_t i = 0; i < ref.length(); i ++) {
		const std::string *val = NULL;
		if (static_cast<unsigned char>(ref[i]) < 128) {
			str += ref[i];
			continue ;
		}
		switch (static_cast<unsigned char>(ref[i]))
		{
		case arg_:
			break ;
		case args:
			break ;
		case query_string:
			break ;
		case content_length:
			str += to_String(client.getContentLength());
			break ;
		case content_type:
			str += client.getContentType();
			break ;
		case document_root:
			break ;
		case document_url:
			break ;
		case host:
			break ;
		case url:
			str += client.getCurrentUri();
			break ;
		case remote_addr:
			break ;
		case remote_port:
			break ;
		case request_method:
			break ;
		case scheme:
			break ;
		case status:
			break ;
		default:
			str += ref[i];
		}
	}
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

EmbeddedVariable::EmbeddedVariable( const EmbeddedVariable &src ) {
	(void)src;
}

EmbeddedVariable&	EmbeddedVariable::operator=( const EmbeddedVariable &src )
{ (void)src; return (*this); }
