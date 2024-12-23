/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:37:55 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/22 20:48:51 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) { }

Location::Location( const Location &src ) : InfoBlock(src) {
	*this = src;
}

Location& Location::operator=( const Location &src ) {
	if (this != &src) {
		InfoBlock::operator=(src);
		path = src.path;
		alias = src.alias;
		internal = src.internal;
		return_ = src.return_;
		cgi_mapping = src.cgi_mapping;
		is_cgi = src.is_cgi;
	}
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
