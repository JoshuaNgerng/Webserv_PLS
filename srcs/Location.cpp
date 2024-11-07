/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:37:55 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/07 21:01:50 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) :
path(""),
alias(""),
internal(false),
return_()
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
	cgi_enabled = src.cgi_enabled;
	cgi_mapping = src.cgi_mapping;
	return_ = src.return_;
	return (*this);
}

Location::~Location( void ) { }

void	Location::routingClient( Client &client ) const {
	// if proxy server and if cgi handle diff
	// if (alias.length() > 0) {
	// 	root = alias; // logic for alias
	// } resolve roo alias before server setup
	InfoBlock::routingClient(client);
}

void	Location::defaultSetting( const InfoBlock &ref ) {
	InfoBlock::defaultSetting(ref);
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
}

const std::string&	Location::getLocationPath( void ) const {
	return (path);
}
