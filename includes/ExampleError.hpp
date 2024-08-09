/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExampleError.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:32:07 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/09 14:49:28 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
this example for ErrorClass for standardization
*/
#include <exception>
#include <string>

// use errorvalue in exception class construction
enum errorvalue {
	example1 = 0,
	example2 = 1,
	example3 = 2
};

class ExampleError : public std::exception
{
	public:
		ExampleError( int val );
		~ExampleError( void ) throw();
		virtual const char *what() const throw();
	private:
		void	msgInit( int val );
		std::string	msg;
};
