/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_fd_array.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:24:16 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/27 16:43:51 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simple_fd_array.hpp"

TestFds::TestFds( void ) : socket_fds(), buffer(), 
	poll_tracker(), fd_counter(), buffer_counter() { }

TestFds::~TestFds( void ) { }

void	TestFds::resetFdsInit( void ) {
	for (size_t index = 0; index < buffer.size(); index ++) {
		if (buffer[index].fd > -1)
			buffer_counter ++;
	}
	size_t	counter = 0;
	for (size_t index = 0; index < socket_fds.size(); index ++) {
		if (socket_fds[index].fd > -1) {
			counter ++;
			poll_tracker = index + 1;
		}
	}
	// std::cout << "test " << buffer_counter << " " << poll_tracker << '\n';
	fd_counter = counter + buffer_counter;
	if (fd_counter >= socket_fds.size())
		throw BufferError("not enough space in socket_fds");
	size_t	index = 0;
	for (; index < buffer_counter; index ++) {
		if (buffer[index].fd == -1)
			break ;
	}
	for (; index < buffer_counter; index ++) {
		if (buffer[index].fd > -1)
			throw BufferError("invalid buffer format");
	}
}

void	TestFds::resetFds( void ) { // main func to test
	resetFdsInit();
	nfds_t	i = 0;
	nfds_t	buffer_index = 0;
	for (i = 0; i < poll_tracker; i ++) {
		if (socket_fds[i].fd > -1) {
			continue ;
		}
		if (buffer_index < buffer_counter) {
			std::swap(buffer[buffer_index], socket_fds[i]);
			buffer_index ++;
		}
		else {
			for (nfds_t	checker = poll_tracker; checker > i; checker --) {
				// std::cout << "//testing " << socket_fds[checker - 1].fd << '\n';
				if (socket_fds[checker - 1].fd > -1) {
					// std::cout << "testing " << socket_fds[checker - 1].fd << '\n';
					std::swap(socket_fds[checker - 1], socket_fds[i]);
					break ;
				}
			}
		}		
	}
	for (; buffer_index < buffer_counter; buffer_index ++) {
		while (i < socket_fds.size())
		{
			if (socket_fds[i].fd == -1)
				break ;
			i ++;
		}
		std::swap(socket_fds[i], buffer[buffer_index]);
	}
	buffer_counter = 0;
	poll_tracker = fd_counter;
}

bool	TestFds::checkResetFds( void ) const {
	size_t	index = 0;
	size_t	check = 0;
	for (; index < socket_fds.size(); index ++) {
		if (socket_fds[index].fd == -1)
			break ;
		check ++;
	}
	for (; index < socket_fds.size(); index ++) {
		if (socket_fds[index].fd != -1 && socket_fds[index].events != POLLIN) {
			return (false);
		}
	}
	if (check != poll_tracker || check != fd_counter || buffer_counter != 0) {
		// std::cout << "testi check " << check << ", poll tracker " << poll_tracker << ", fd counter " << fd_counter << '\n';
		return (false);
	}
	index = 0;
	for (; index < buffer.size(); index ++) {
		if (buffer[index].fd != -1 || buffer[index].events != POLLIN)
			return (false);
	}
	return (true);
}

void	TestFds::setSocketFds( const std::vector<pollfd_t>& ref  ) {
	for (size_t i = 0; i < ref.size(); i ++) {
		socket_fds.push_back(ref[i]);
	}
}

void	TestFds::setBuffer( const std::vector<pollfd_t>& ref ) {
	for (size_t i = 0; i < ref.size(); i ++) {
		buffer.push_back(ref[i]);
	}
}

std::ostream&	TestFds::displaySocketFds( std::ostream& o ) const {
	return (displayContainer(o, socket_fds));
}

std::ostream&	TestFds::displayBuffer( std::ostream& o ) const {
	return (displayContainer(o, buffer));
}

std::ostream&	TestFds::displaySimple( std::ostream& o ) const {
	o << "{ ";
	for (size_t index = 0; index < socket_fds.size(); index ++) {
		o << socket_fds[index].fd << ", ";
	}
	o << "}\n";
	return (o);
}

nfds_t	TestFds::getPollTracker( void ) const { return (poll_tracker); }

nfds_t	TestFds::getFdCounter( void ) const { return (fd_counter); }

nfds_t	TestFds::getBufferCounter( void ) const { return (buffer_counter); }

std::ostream&	operator<<( std::ostream &o, const pollfd_t& src ) {
	o << "fd: " << src.fd << ", events: ";
	if (src.events & POLLIN)
		o << "POLLIN ";
	if (src.events & POLLOUT)
		o << "POLLOUT ";
	if (!(src.events & (POLLIN | POLLOUT)))
		o << "UNKNOWN ";
	return (o);
}

std::ostream&	operator<<( std::ostream &o, const TestFds& src ) {
	o << "Socket fds: ";
	src.displaySocketFds(o) << '\n';
	o << "Buffer fds: ";
	src.displayBuffer(o);
	return (o);
}

TestFds::BufferError::BufferError( const char *str ) : msg(str) {}

TestFds::BufferError::BufferError( const std::string &msg_ ) : msg(msg_) {}

TestFds::BufferError::~BufferError( void ) throw() {}

const char* TestFds::BufferError::what( void ) const throw() { return(msg.c_str()); }
