/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EmbeddedVariable.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 04:31:35 by joshua            #+#    #+#             */
/*   Updated: 2024/11/07 15:57:32 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EmbeddedVariable.hpp"
#include "Client.hpp"

const char	*EmbeddedVariable::variables[] = {
	"arg_", "args", "query_string", "content_length", "content_type",
	"document_root", "document_url", "host", "url",
	"remote_addr", "remote_port", "request_method",
	"scheme", NULL
};

const uint8_t	EmbeddedVariable::shortform[] = {
	arg_, args, query_string, content_length, content_type,
	document_root, document_uri, document_url, host, uri, url,
	remote_addr, remote_port, request_method, scheme
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

static void	getQueryArgs( std::string &dst, const std::string &uri ) {
	size_t	pos = uri.find('?');
	if (pos == std::string::npos) {
		return ;
	}
	dst.append(uri, pos + 1);
}

static void	extractArg(
	std::string &buffer, const std::string &arg_name,
	size_t start, const char *delim
) {
	// const char ref[] = "\'\"$";
	for (size_t i = start; i < arg_name.length(); i ++) {
		for (size_t j = 0; delim[j]; j ++) {
			if (arg_name[i] == delim[j]) {
				buffer = arg_name.substr(start, i - start);
				return ;
			}
		}
	}
	buffer = arg_name.substr(start);
}



static int	getQueyArgs( std::string &dst, const std::string &arg_name, size_t start, const std::string &uri ) {
	std::string	buffer_name, buffer_uri, buffer_result;
	extractArg(buffer_name, arg_name, start, "\'\"$");
	buffer_name.append('=', 1);
	getQueryArgs(buffer_uri, uri);
	if (!buffer_uri.length()) {
		return (buffer_name.length() - 1);
	}
	extractArg(buffer_result, buffer_uri, 0, ";/?=&#()[]@!$&\'*+,:_")
	return (buffer_name.length() - 1);
}

void	EmbeddedVariable::resolveString( std::string &str, const std::string &ref, const Client &client ) {
	str.reserve(ref.length());
	for (size_t i = 0; i < ref.length(); i ++) {
		const std::string *val = NULL;
		if (ref[i] == '\'' || ref[i] == '"') {
			continue ;
		}
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
			str += client.getRoot();
			break ;
		case document_url:
			break ;
		case host:
			str += client.getHost();
			break ;
		case url:
			str += client.getCurrentUri();
			break ;
		case remote_addr:
			str += client.getAddr();
			break ;
		case remote_port:
			str += to_String(client.getPort());
			break ;
		case request_method:
			break ;
		case scheme:
			str += client.getHttpScheme();
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
