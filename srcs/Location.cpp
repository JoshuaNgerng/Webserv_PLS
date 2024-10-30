/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:37:55 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/30 16:33:06 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) :
path(""),
alias(""),
internal(false),
cgi_mapping(),
limit_except(),
return_(),
is_cgi(false)
{ }

Location::Location( const Location &src ) : InfoBlock(src) {
	*this = src;
}

Location& Location::operator=( const Location &src ) {
	if (this == &src) {
		return (*this);
	}
	InfoBlock::operator=(src);
	path = src.path;
	alias = src.alias;
	internal = src.internal;
	return_ = src.return_;
	cgi_mapping = src.cgi_mapping;
	is_cgi = src.is_cgi;
	return (*this);
}

Location::~Location( void ) { }

void	Location::matchUri( Client &client, bool autoindex_ ) const {
	(void)client;
	(void)autoindex_;
}

void	Location::addPath( const std::string &path_ ) {
	path = path_;
}

void	Location::addAlias( const std::string &path_ ) {
	alias = path_;
}

void	Location::setInternal( void ) {
	internal = true;
}

void	Location::reset( void ) {
	InfoBlock::reset();
	path.clear();
	alias.clear();
	internal = false;
	return_.first = 0;
	return_.second.clear();
	cgi_mapping.clear();
	is_cgi = false;
}

const std::string&	Location::getLocationPath( void ) const {
	return (path);
}
