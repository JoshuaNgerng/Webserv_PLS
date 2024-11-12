/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:09:31 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/05 18:01:26 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticFile.hpp"

StaticFile::StaticFile( void ) : File() { }

StaticFile::StaticFile( const StaticFile &src ) : File(src) { }

StaticFile::~StaticFile( void ) {  }

StaticFile&	StaticFile::operator=( const StaticFile &src ) {
	File::operator=(src);
	return (*this);
}

bool	StaticFile::generateFds( void ) {
	content_input_fd = open(content_id.c_str(), O_RDONLY);
	if (content_input_fd < 0) {
		return (false);
	}
	counter = 1;
	return (true);
}

bool	StaticFile::checkStatus( void )  { return (true); }

std::ostream&	StaticFile::print( std::ostream &o ) const{
	o << "StaticFile status, path " << content_id << "\n";
	if (content_input_fd > 0) {
		o << "fd: " << content_input_fd << '\n';
	} else {
		o << "No fd made\n";
	}
	return (o);
}
