/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Const.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 18:33:17 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/08 20:37:24 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONST_HPP
# define CONST_HPP
# include <iostream>
# include <fstream>
# include <istream>
# include <sstream>
# include <string>
# include <cstring>
# include <vector>
# include "cheader.h"

#define RESET		"\033[0m"
#define BOLDCYAN	"\033[1m\033[36m"
#define BOLDYELLOW	"\033[1m\033[33m"
#define BOLDGREEN	"\033[1m\033[32m"
#define BOLDBLUE	"\033[1m\033[34m"
#define BOLDRED		"\033[1m\033[31m"
#define BOLDMAGENTA	"\033[1m\033[35m"

template< typename T >
std::string	to_String( const T &val )
{
	std::ostringstream	oss;
	oss << val;
	return (oss.str());
}

#endif