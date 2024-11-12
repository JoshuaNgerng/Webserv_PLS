/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:22:51 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/11 23:32:48 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "File.hpp"

File::File( void )
	: content_id(""),
	  already_retrieved_fd(false),
	  content_input_fd(-1),
	  content_output_fd(-1),
	  counter(0)
{ }

File::File( const File &src ) { *this = src; }

File&	File::operator=( const File &src ) {
	if (this == &src) {
		return (*this);
	}
	content_id = src.content_id;
	already_retrieved_fd = src.already_retrieved_fd;
	content_input_fd = src.content_input_fd;
	content_output_fd = src.content_output_fd;
	return (*this);
}

File::~File( void ) {
	if (content_input_fd > 0) {
		close(content_input_fd);
	}
	if (content_output_fd > 0 && content_output_fd != content_input_fd) {
		close(content_output_fd);
	}
}

int	File::getInputFd( void ) const { return (content_input_fd); }

int	File::getOutputFd( void ) const { return (content_output_fd); }

int	File::getFdCounter( void ) const { return (counter); }

const std::string&	File::getContentId( void ) const { return(content_id); }

void	File::setContentPath( const std::string &root, const std::string &fname ) {
	content_id = root + fname;
}

void	File::setContentPath( const std::string &path ) {
	content_id = path;
}

void	File::serverReceivedFds( void ) { already_retrieved_fd = true; }

bool	File::getFds( void ) const { return (already_retrieved_fd); }

void	File::getFds( int fds[2] ) const {
	fds[0] = content_input_fd;
	fds[1] = content_output_fd;
}

bool	File::processFd( int &fd ) const {
	// (void)fd;
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
		close(fd);
		fd = -1;
		return (false);
	}
	return (true);
}

bool	File::processFds( void ) {
	if (!generateFds()) {
		return (false);
	}
	if (content_input_fd > 0 && !processFd(content_input_fd)) {
		return (false);
	}
	if (content_output_fd > 0 && !processFd(content_output_fd)) {
		return (false);
	}
	return (true);
}

std::ostream&	operator<<( std::ostream &o, const File &ref ) {
	ref.print(o);
	return (o);
}
