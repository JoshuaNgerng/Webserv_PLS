/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultErrorPage.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 00:09:55 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/23 13:07:50 by jngerng          ###   ########.fr       */
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
