/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultErrorPage.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 00:09:55 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/08 03:27:36 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFAULTERRORPAGE_HPP
# define DEFAULTERRORPAGE_HPP
# include "Const.hpp"

class DefaultErrorPage
{
public:
	DefaultErrorPage( void );
    DefaultErrorPage( const DefaultErrorPage &src );
    DefaultErrorPage& operator=( const DefaultErrorPage &src );
	~DefaultErrorPage( void );
	static std::string	generateHtml( int status, const char *server_name );
private:
	// put the other sht here
	static const char 	*template_html;
};

#endif