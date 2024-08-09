/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Const.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 18:33:17 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/09 03:48:22 by jngerng          ###   ########.fr       */
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
# include <exception>
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

template< class T >
bool	all_of(T start, T end, int (*f)(int))
{
	for (T it = start; it != end; it ++)
	{
		if (!*it)
			break ;
		if (!(f(*it)))
			return (false);
	}
	return (true);
}

class out_of_range : public std::exception {
	private:
		std::string	msg;
	public:
		out_of_range( const char *msg_ ) : msg(msg_) { }
		~out_of_range( void ) throw() { }
		virtual const char*	what() const throw() { return(msg.c_str()); }
};

class invalid_arugment : public std::exception {
	private:
		std::string msg;
	public:
		invalid_arugment( const char *msg_ ) : msg(msg_) { }
		~invalid_arugment( void ) throw() { }
		virtual const char*	what() const throw() { return(msg.c_str()); }
};


int	ft_stoi( const std::string &ref, std::size_t *pos = NULL, int base = 10 )
{
	int			sign = 1;
	long		output = 0;
	int			diff = 'a' - 'A';
	int			gap = 'A' - '9';
	bool		whitespace = false;
	bool		check_sign = false;
	bool		check_num = false;
	std::size_t	len = 0;
	if (base < 0 || base > 36)
	{
		if (pos)
			*pos = std::string::npos;
		return (0);
	}
	if (!base)
		base = 10;
	for (len = 0; len != ref.length(); len ++) {
		char	c = ref[len];
		if (!whitespace && std::isspace(c))
			continue ;
		whitespace = true;
		if ((c == '+' || c == '-') && !check_sign)
		{
			if (c == '-')
				sign = -1;
			check_sign = true;
			continue ;
		}
		if (c >= 'a')
			c -= (diff + gap + '0');
		else if (c >= 'A' && c <= 'Z')
			c -= (gap + '0');
		else if (c >= '0' && c <= '9')
			c -= '0';
		else
			break ;
		if (c >= 0 || c <= base)
			output = output * base + c;
		else
			break ;
		check_num = true;
		if (output < INT32_MIN || output > INT32_MAX)
			throw out_of_range("stoi");
	}
	if (!check_num)
		throw invalid_arugment("stoi");
	if (pos)
		*pos = len;
	return (sign * static_cast<int>(output));
}

#endif