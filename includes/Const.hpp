/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Const.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 18:33:17 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/12 23:55:07 by jngerng          ###   ########.fr       */
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
# include <cstdlib>
# include <csignal>
# include <ctime>
# include <exception>
# include <stdexcept>
# include <list>
# include <vector>
# include <queue>
# include <map>
# include <utility>
# include "cheader.h"

#define RESET		"\033[0m"
#define BOLDCYAN	"\033[1m\033[36m"
#define BOLDYELLOW	"\033[1m\033[33m"
#define BOLDGREEN	"\033[1m\033[32m"
#define BOLDBLUE	"\033[1m\033[34m"
#define BOLDRED		"\033[1m\033[31m"
#define BOLDMAGENTA	"\033[1m\033[35m"

enum boolean {
	undefined,
	on,
	off
};

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

template< class T >
std::ostream&	displayContainer( std::ostream &o, const T &ref ) {
	for (typename T::const_iterator it = ref.begin(); it != ref.end(); it ++) {
		o << *it << ' ';
	}
	return (o);
}

template< class T >
std::ostream&	displayContainer( std::ostream &o, const T &ref, const std::string &end ) {
	for (typename T::const_iterator it = ref.begin(); it != ref.end(); it ++) {
		o << *it << end;
	}
	return (o);
}

template< class T >
std::ostream&	displayContainer( std::ostream &o, const T &ref, const char *end ) {
	for (typename T::const_iterator it = ref.begin(); it != ref.end(); it ++) {
		o << *it << end;
	}
	return (o);
}

template <typename T>
void	swap_clear(T &a)
{
	T	buffer;
	std::swap(a, buffer);
}

int		ft_strncpy(const char *s1, const char *s2, size_t n);
size_t	ft_strstr(const char *haystack, const char *needle);

template< class C >
class Display {
	typedef std::ostream& (C::*display_func)( std::ostream &o ) const; 
	public:
		Display( const C &src, display_func display_ ) : class_ptr(&src), display(display_) { }
		~Display( void ) { }
		const C			&class_ptr;
		display_func	display;
	private:
		Display( void ) { }
		Display( const Display &src ) : class_ptr(src.class_ptr), display(src.display) { }
		Display&	operator=( const Display &src ) { display = src.display; return (*this); }
};

template< class C >
std::ostream&	operator<<( std::ostream &o, const Display<C> &d ) {
	return (d.class_ptr->*(d.display(o)));
}

class InfoBlock;
class ServerInfo;
class Location;
class Client;
class HttpRequest;
class LimitExcept;

#endif
