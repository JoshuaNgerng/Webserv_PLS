/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 22:30:09 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/05 22:44:20 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
# define ERRORPAGE_HPP
# include "Const.hpp"

class ErrorPage {
	public:
		ErrorPage( void );
		~ErrorPage( void );

		bool	inputStr( const std::string &token );
		bool	findError( std::string &path_, int status );
	private:
		std::vector<int>    codes;
		std::string         path;
};

#endif