/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 22:30:09 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/05 23:05:08 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
# define ERRORPAGE_HPP
# include "Const.hpp"

class ErrorPage {
	public:
		ErrorPage( void );
		ErrorPage( const ErrorPage &src );
		~ErrorPage( void );

		ErrorPage&	operator=( const ErrorPage &src );

		bool	inputStr( const std::string &token );
		bool	findError( std::string &path_, int status ) const;
	private:
		std::vector<short>    codes;
		std::string         path;
};

#endif