/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingError.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:40:53 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/19 11:04:30 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSINGERROR_HPP
# define PARSINGERROR_HPP
#include <exception>
#include <string>

// use errorvalue in exception class construction
enum parsingerrorval {
	file_type = 0,
	file_exten = 1,
	file_open = 2,
	file_empty = 3,
	unclosed_bracket = 4,
	unknown_option = 5,
	repeated_port = 6,
	invalid_ip_add = 7,
	delimitor_not_found = 8
};

class ParsingError : public std::exception
{
	public:
		ParsingError( int val );
		~ParsingError( void ) throw();
		virtual const char *what() const throw();
	private:
		void	msgInit( int val );
		std::string	msg;
};

#endif
