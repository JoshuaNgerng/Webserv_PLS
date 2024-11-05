/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:22:51 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/05 10:54:20 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "File.hpp"

File::File( void )
	: content_fd(-1),
	  content_path(""),
	  already_retrieved_fd(false)
{ }

File::File( const File &src )
	: content_fd(src.content_fd),
	  content_path(src.content_path),
	  already_retrieved_fd(src.already_retrieved_fd)
{ }

File&	File::operator=( const File &src ) {
	if (this == &src) {
		return (*this);
	}
	content_fd = src.content_fd;
	content_path = src.content_path;
	already_retrieved_fd = src.already_retrieved_fd;
	return (*this);
}

File::~File( void ) { if (content_fd > 0) { close(content_fd); } }

void	File::setContentPath( const std::string &root, const std::string &fname ) {
	content_path = root + fname;
}

bool	File::getFd( void ) const { return (already_retrieved_fd); }

void	File::getFd( int fds[2] ) const {
	fds[0] = content_fd;
	fds[1] = -1;
}

bool	File::makeFd(void) {
	if (content_fd > 0) {
		close(content_fd);
	}
	content_fd = open(content_path.c_str(), O_RDONLY);
	if (content_fd < 0) {
		return (false);
	}
	return (true);
}
