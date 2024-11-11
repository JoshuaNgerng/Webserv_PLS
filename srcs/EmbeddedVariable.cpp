/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EmbeddedVariable.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 04:31:35 by joshua            #+#    #+#             */
/*   Updated: 2024/11/08 19:39:35 by jngerng          ###   ########.fr       */
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

static int	getQueyArgs( std::string &dst, const std::string &arg_name, size_t start, const std::string &query ) {
	std::string	buffer_name, buffer_result;
	extractArg(buffer_name, arg_name, start, "\'\"$");
	buffer_name.append('=', 1);
	if (!query.length()) {
		return (buffer_name.length() - 1);
	}
	size_t pos = query.find(buffer_name);
	if (pos == std::string::npos) {
		return (buffer_name.length() - 1);
	}
	extractArg(buffer_result, query, pos + buffer_name.length(), ";/?=&#()[]@!$&\'*+,:_");
	dst += EmbeddedVariable::decodeUrl(buffer_result); // decode url
	return (buffer_name.length() - 1);
}

void	EmbeddedVariable::resolveString( std::string &str, const std::string &ref, const Client &client ) {
	str.reserve(ref.length());
	for (size_t i = 0; i < ref.length(); i ++) {
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
			i += getQueyArgs(str, ref, i, client.getCurrentQuery());
			break ;
		case args:
			str += client.getCurrentQuery();
			break ;
		case query_string:
			str += client.getCurrentQuery();
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
			str += client.getReqMethod();
			break ;
		case scheme:
			str += client.getHttpScheme();
			break ;
		default:
			str += ref[i];
		}
	}
}

static bool	checkhex( const std::string &str, size_t start, size_t lim ) {
	for (size_t i = 0; i < lim; i ++) {
		if (start + i >= str.length()) {
			return (false);
		}
		int check = static_cast<int>(str[start + i]);
		if (!std::isdigit(check)) {
			if (!(check >= 'a' && check <= 'f')) {
				if (!(check >= 'A' && check <= 'F')) {
					return (false);
				}
			}
		}
	}
	return (true);
}

static char	hex2alpha( const std::string &str, size_t start ) {
	int	c = 0;
	for (int i = 0; i < 2; i ++) {
		char	check = static_cast<int>(str[start + i]);
		c *= 16;
		if (check >= 'a' && check <= 'f') {
			c += 10 + check - 'a' + 1;
		} else if (check >= 'A' && check <= 'F') {
			c += 10 + check - 'A' + 1;
		} else {
			c += check;
		}
	}
	return (static_cast<char>(c));
}

bool	EmbeddedVariable::checkUrl( const std::string &url ) {
	for (size_t i = 0; i < url.length(); i ++) {
		if (url[i] == '$') {
			if (!checkhex(url, i + 1, 2)) {
				return (false);
			}
			i += 2;
			continue ;
		}
	}
	return (true);
}

std::string	EmbeddedVariable::decodeUrl( const std::string &url ) {
	std::string	result;
	result.reserve(url.length());
	for (size_t i = 0; i < url.length(); i ++) {
		if (url[i] != '$') {
			result += url[i];
			continue ;
		}
		result += hex2alpha(url, i);
		i += 2;
	}
	return (result);
}

EmbeddedVariable::EmbeddedVariable( void ) { }

EmbeddedVariable::EmbeddedVariable( const EmbeddedVariable &src ) {
	(void)src;
}

EmbeddedVariable&	EmbeddedVariable::operator=( const EmbeddedVariable &src )
{ (void)src; return (*this); }
