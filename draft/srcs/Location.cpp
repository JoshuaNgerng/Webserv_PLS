/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:37:55 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/24 17:33:11 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) { }

Location::Location( const std::string &path ) : InfoBlock(), path(path) { }

Location::~Location( void ) { }

void	Location::reset( void ) {
	InfoBlock::reset();
	path.clear();
	alias.clear();
	internal = false;
	is_cgi = false;
}
