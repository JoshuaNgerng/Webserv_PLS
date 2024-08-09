/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExampleError.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:39:23 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/09 14:49:34 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExampleError.hpp"

ExampleError::ExampleError( int val ) { msgInit(val); }

ExampleError::~ExampleError( void ) { }

void	ExampleError::msgInit( int val ) {
	msg = "Error:";
	switch (val)
	{
	case example1:
		msg += "1 errormsg";
		break;
	case example2:
		msg += "2 msg";
		break;
	case example3:
		msg += "3 lulz";
		break ;
	default:
		msg += "Unknown Error";
		break;
	}
}

const char*	ExampleError::what() const throw() { return (msg.c_str()); }
