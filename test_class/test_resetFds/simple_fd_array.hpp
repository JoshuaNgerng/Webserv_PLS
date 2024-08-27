/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_fd_array.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:10:35 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/27 16:51:34 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLE
# define SIMPLE
# include "Const.hpp"

/**
 * @brief	just for testing purposes only
 * 			main func to test is restFds
*/
class TestFds
{
private:
	std::vector<pollfd_t>	socket_fds;
	std::vector<pollfd_t>	buffer;
	nfds_t					poll_tracker;
	nfds_t					fd_counter;
	nfds_t					buffer_counter;

	void	resetFdsInit( void );

public:
	TestFds( void );
	~TestFds( void );

	void	resetFds( void ); // main func to test
	bool	checkResetFds( void ) const; // main func to test whether resetFds is correct
	// if resetFds is correct it return true esle return false

	void	setSocketFds( const std::vector<pollfd_t>& ref );
	void	setBuffer( const std::vector<pollfd_t>& ref );

	/*	display function are the public func to display info
		you can modify this func to modify display
	*/
	std::ostream&	displaySocketFds( std::ostream& o ) const;
	std::ostream&	displayBuffer( std::ostream& o ) const;
	std::ostream&	displaySimple( std::ostream& o ) const;

	nfds_t	getPollTracker( void ) const;
	nfds_t	getFdCounter( void ) const;
	nfds_t	getBufferCounter( void ) const;

	class BufferError : public std::exception {
		private:
			std::string	msg;
			// void	msgInit( const std::string &msg_ );

		public:
			BufferError( const char *str );
			BufferError( const std::string &msg_ );
			~BufferError( void ) throw();
			virtual const char *what( void ) const throw();
	};
};

// operator << for easier debugging and display class info
// modify if you want info to display differently

std::ostream&	operator<<( std::ostream &o, const pollfd_t& src );

std::ostream&	operator<<( std::ostream &o, const TestFds& src );

#endif