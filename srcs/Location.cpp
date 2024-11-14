/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:37:55 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/14 21:59:45 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Client.hpp"

Location::Location( void ) :
path(""),
internal(false),
return_()
{ }

Location::Location( const Location &src ) : InfoBlock() {
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
	return (*this);
}

Location::~Location( void ) { }

void Location::routeClientReturn( Client &client ) const {
	int check = return_.first;
	if (check > 299 && check < 400) {
		client.addContent(check, return_.second);
		return ;
	}
	client.addContent(check);
	// client. add Body? COUT
}

void	Location::routingClient( Client &client ) const {
	if (internal) {
		client.addContent(403);
		return ;
	}
	if (return_.first > 0) {
		routeClientReturn(client);
		return ;
	}
	InfoBlock::routingClient(client, path);
}

void	Location::defaultSetting( const InfoBlock &ref ) {
	InfoBlock::defaultSetting(ref);
}

void	Location::addPath( const std::string &path_ ) {
	path = path_;
}

void	Location::addAlias( const std::string &path_ ) {
	alias = true;
	if (root.length()) {
		std::invalid_argument("root and alias conflict");
	}
	root = path_;
}

void	Location::setInternal( void ) { internal = true; }

void	Location::addReturn( int code ) { return_.first = code; }

void	Location::addReturn( const std::string &uri ) { return_.second = uri; }

void	Location::addReturn( int code, const std::string &uri ) {
	addReturn(code);
	addReturn(uri);
}

void	Location::reset( void ) {
	InfoBlock::reset();
	path.clear();
	internal = false;
	return_.first = 0;
	return_.second.clear();
}

int	Location::checkReturnCode( void ) const { return (return_.first); }

const std::string&	Location::checkReturnUri( void ) const {
	return (return_.second);
}

const std::string&	Location::getLocationPath( void ) const {
	return (path);
}
