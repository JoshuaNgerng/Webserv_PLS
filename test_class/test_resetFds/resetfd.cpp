/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resetfd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:49:51 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/14 22:08:38 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simple_fd_array.hpp"
/*
like fds  -1 (and negative numbers) are invalid fds
resetFds suppose to compress the socketfds array
before 
{1, 2, -1, 3, -1, 4, -1, -1, -1, 5}
after
{1, 2, 3, 4, 5, -1, -1, -1, -1, -1}
{all the valid fds, all the invalid fds}
similar to subnet bit mask 1111100000 ex invalid bit mask 111101110111

resetFds will also add any fds in buffer into socket
before
sockets
{1, 2, -1, -1, 3, -1, 4, -1}
buffer
{5, 6, -1, -1, -1}
after
{1, 2, 5, 6, 3, 4, -1, -1, -1}
{-1, -1, -1, -1, -1}
invalid buffer ex {-1, 1, 2, -1, 3} will throw an error
but its dont test that cause the buffer error handling is other part of the class
not change the events in pollfd_t of buffer not change invalid fd events
only change events in sockets_fd
also dont test when theres not enough space in socket_fds
ex
sockets
{1, 2, -1, -1,} 
buffer
{3, 4, 5, -1}
this will also throw error
this test cases that are valid will not throw error
you check wheter the function by using checkResetFds
if it returns true it works , false it doesnt work
*/

class	InvalidOperation : public std::exception {
	public:
		virtual const char *what( void ) const throw() {
			return ("Invalid operation in main");
		}
};

/**
 * @brief	change from int arr into vector so its easier to pass into
 * 			TestFds class by default everything is initlized with events as POLLIN
*/
std::vector<pollfd_t>	make_pollfd( int arr[], size_t size ) {
	std::vector<pollfd_t>	out;
	pollfd_t	fd;
	fd.fd = -1;
	fd.events = POLLIN;
	fd.revents = 0;
	for (size_t i = 0; i < size; i ++) {
		fd.fd = arr[i];
		out.push_back(fd);
	}
	return (out);
}

/**
 * @brief	change a specfic element in vector (in socket_fds)
 * 			by default everything in vector have events member as POLLIN
 * 			but during production the events can be changes to POLLOUT
 * 
 * @attention	only modify socket_fds , not change events of -1 fd will throw error
*/
void	changeEventToPOLLOUT( std::vector<pollfd_t> &src, size_t index ) {
	if (src[index].fd == -1)
		throw InvalidOperation();
	src[index].events = POLLOUT;
}

void	test_case1( void ) {
	int	socket_fds[] = {1, 2, 3, -1, 5, -1, -1, -1, 6, -1, -1, -1, -1, -1};
	size_t	len1 = (sizeof(socket_fds)/sizeof(socket_fds[0]));
	int	buffer[] = {7, 8, 9, 10, 11, -1, -1};
	size_t	len2 = (sizeof(buffer)/sizeof(buffer[0]));
	TestFds	test;
	test.setSocketFds(make_pollfd(socket_fds, len1));
	test.setBuffer(make_pollfd(buffer, len2));
	test.resetFds();
	test.displaySimple(std::cout);
}

void	test_case2( void ) {
	int	socket_fds[] = {1, 2, 3, -1, 5, -1, -1, -1, 6, -1, -1, -1, -1, -1};
	size_t	len1 = (sizeof(socket_fds)/sizeof(socket_fds[0]));
	int	buffer[] = {-1, -1, -1, -1, -1, -1, -1};
	size_t	len2 = (sizeof(buffer)/sizeof(buffer[0]));
	TestFds	test;
	test.setSocketFds(make_pollfd(socket_fds, len1));
	test.setBuffer(make_pollfd(buffer, len2));
	test.resetFds();
	std::cout << ((test.checkResetFds()) ? "success" : "failure") << '\n';
	std::cout << test << '\n';
	test.displaySimple(std::cout);
}

int	main(void) {
	std::cout << BOLDBLUE << "test 1" << RESET "\n";
	test_case1();
	std::cout << BOLDBLUE << "test 2" << RESET "\n";
	test_case2();
	return (0);
}