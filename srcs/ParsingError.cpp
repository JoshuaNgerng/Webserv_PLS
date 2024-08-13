/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingError.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:40:26 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/13 17:12:08 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsingError.hpp"

ParsingError::ParsingError( int val ) { msgInit(val); }

ParsingError::~ParsingError( void ) throw() { }

void	ParsingError::msgInit( int val ) {
	msg = "ParsingError: ";
	switch (val)
	{
	case file_type:
		msg += "File cannot be accessed";
		break ;
	case file_exten:
		msg += "Wrong File extension";
		break ;
	case file_open:
		msg += "File cannot be opened";
		break ;
	case file_empty:
		msg += "File empty";
		break ;
	case unclosed_bracket:
		msg += "Brackets not closed";
		break ;
	default:
		msg += "Unknown Error";
		break ;
	}
}

const char*	ParsingError::what() const throw() { return (msg.c_str()); }
