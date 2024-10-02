/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultErrorPage.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 00:09:55 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/02 01:44:47 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFAULTERRORPAGE_HPP
# define DEFAULTERRORPAGE_HPP
# include "Const.hpp"

class DefaultErrorPage
{
public:
	~DefaultErrorPage( void );
	static std::string	generateHtml( int status, const char *server_name );
private:
	DefaultErrorPage( void );
	// put the other sht here
	static const char 	*template_html;
};

#endif