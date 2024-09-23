/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:58:44 by joshua            #+#    #+#             */
/*   Updated: 2024/09/19 01:54:33 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPage.hpp"

const char	*ErrorPage::default_template =
"<html>\n"
"<head><title>*</title></head>\n"
"<body>\n"
"<center><h1>*</h1></center>\n"
"<hr><center>webserv (#)</center>\n"
"</body>\n"
"</html>\n";

ErrorPage::ErrorPage( void ) { }

ErrorPage::~ErrorPage( void ) { }

void	ErrorPage::addErrorPage( int status_code, const std::string &path )
{ errmsg_config[status_code] = path; }

bool	ErrorPage::searchErrorPage( std::string &path, int status_code ) const{
	typedef std::map<int, std::string>::const_iterator iter;
	iter it = errmsg_config.find(status_code);
	if (it == errmsg_config.end())
		return (false);
	path = it->second;
	return (true);
}

std::string	ErrorPage::makeDefaultMsg( int status_code,
			const char *status_msg, const char *distro = NULL ) const {
	std::string out(default_template);
	std::string	buffer(status_msg);
	buffer = ' ' + buffer;
	buffer = to_String(status_code) + buffer;
	size_t pos = out.find('*');
	out.replace(pos, 1, buffer);
	pos = out.find('*', pos);
	out.replace(pos, 1, buffer);
	if (!distro) {
		out.erase(out.find('(') - 1, 4);
		return (out);
	}
	out.replace(out.find('#'), 1, distro);
	return (out);
}
