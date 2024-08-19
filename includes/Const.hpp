/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Const.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 18:33:17 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/15 23:40:32 by jngerng          ###   ########.fr       */
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
# include <list>
# include <vector>
# include <map>
# include <utility>
# include "cheader.h"
# include "ParsingError.hpp"

#define RESET		"\033[0m"
#define BOLDCYAN	"\033[1m\033[36m"
#define BOLDYELLOW	"\033[1m\033[33m"
#define BOLDGREEN	"\033[1m\033[32m"
#define BOLDBLUE	"\033[1m\033[34m"
#define BOLDRED		"\033[1m\033[31m"
#define BOLDMAGENTA	"\033[1m\033[35m"

/**
 * @brief	creating std::to_string in c++98
 * 			if there is a defined operator << into ostringstream
 * 			then it will use that operator to convert to string
 * 
 * @param	val	the object you want to convert into string form
 * @return	a new string representation
 * 
 * @attention	if no << defined it should have a complie error
 */
template< typename T >
std::string	to_String( const T &val )
{
	std::ostringstream	oss;
	oss << val;
	return (oss.str());
}

/**
 * @brief	creating std::all_of in c++98
 * 			used as a generic way to check every element between start and end
 * 			whether they fulfill some condition in func f
 * 
 * @param	start	the start iterator 
 * @param	end		the end iterator
 * 
 * @attention	if T is not iterator type it will have undefined behaviour
 * 				if start does not reach the end it will also have undefined behaviour
 */
template< typename T >
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

/**
 * @brief	creating std::move in c++98
 * 			move contents of src to dst then empty the content of src
 * @param	dst the object you want to move to
 * @param	src the object you want to move from
 * 
 * @attention	if dst is not empty the deconstructor in temp will 
 * 				erase whatever memory is in dst
 * 				no tested yet lulz
 */
template< typename T >
void	ft_move( T &dst, T &src ) {
	T	temp;
	std::swap(dst, src);
	std::swap(src, temp);
}

// class out_of_range : public std::exception {
// 	private:
// 		std::string	msg;
// 	public:
// 		out_of_range( const char *msg_ ) : msg(msg_) { }
// 		~out_of_range( void ) throw() { }
// 		virtual const char*	what() const throw() { return(msg.c_str()); }
// };

// class invalid_arugment : public std::exception {
// 	private:
// 		std::string msg;
// 	public:
// 		invalid_arugment( const char *msg_ ) : msg(msg_) { }
// 		~invalid_arugment( void ) throw() { }
// 		virtual const char*	what() const throw() { return(msg.c_str()); }
// };


// int	ft_stoi( const std::string &ref, std::size_t *pos = NULL, int base = 10 )
// {
// 	int			sign = 1;
// 	long		output = 0;
// 	int			diff = 'a' - 'A';
// 	int			gap = 'A' - '9';
// 	bool		whitespace = false;
// 	bool		check_sign = false;
// 	bool		check_num = false;
// 	std::size_t	len = 0;
// 	if (base < 0 || base > 36)
// 	{
// 		if (pos)
// 			*pos = std::string::npos;
// 		return (0);
// 	}
// 	if (!base)
// 		base = 10;
// 	for (len = 0; len != ref.length(); len ++) {
// 		char	c = ref[len];
// 		if (!whitespace && std::isspace(c))
// 			continue ;
// 		whitespace = true;
// 		if ((c == '+' || c == '-') && !check_sign)
// 		{
// 			if (c == '-')
// 				sign = -1;
// 			check_sign = true;
// 			continue ;
// 		}
// 		if (c >= 'a')
// 			c -= (diff + gap + '0');
// 		else if (c >= 'A' && c <= 'Z')
// 			c -= (gap + '0');
// 		else if (c >= '0' && c <= '9')
// 			c -= '0';
// 		else
// 			break ;
// 		if (c >= 0 || c <= base)
// 			output = output * base + c;
// 		else
// 			break ;
// 		check_num = true;
// 		if (output < INT32_MIN || output > INT32_MAX)
// 			throw out_of_range("stoi");
// 	}
// 	if (!check_num)
// 		throw invalid_arugment("stoi");
// 	if (pos)
// 		*pos = len;
// 	return (sign * static_cast<int>(output));
// }

#endif
