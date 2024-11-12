/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultErrorPage.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 00:38:44 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/31 23:18:22 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DefaultErrorPage.hpp"
#include "Http.hpp"

const char *DefaultErrorPage::template_html = 
"<html>\n"
"<head><title>*</title></head>\n"
"<body>\n"
"<center><h1>*</h1></center>\n"
"<hr><center> # </center>\n"
"</body>\n"
"</html>\n";

DefaultErrorPage::DefaultErrorPage( void ) { }

DefaultErrorPage::DefaultErrorPage( const DefaultErrorPage &src ) {
	(void)src;
}

DefaultErrorPage& DefaultErrorPage::operator=( const DefaultErrorPage &src ) {
	(void)src; return *this;
}

DefaultErrorPage::~DefaultErrorPage( void ) { }

std::string	DefaultErrorPage::generateHtml( int status, const char *server_name = NULL ) {
	const char *msg = Http::fetchMsg(status);
	std::string	out(template_html);
	std::string str(" ");
	str += to_String(status) + ' ';
	if (!msg)
		msg = "Unknown Error";
	str += msg;
	str += ' ';
	size_t pos = out.find('*');
	out.replace(pos, 1, str);
	pos = out.find('*', pos);
	out.replace(pos, 1, str);
	pos = out.find('#', pos);
	if (!server_name)
		out.erase(pos - 1, 3);
	else
		out.replace(pos, 1, server_name);
	return (out);
}
