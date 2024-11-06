/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 22:39:42 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/05 22:44:12 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPage.hpp"

ErrorPage::ErrorPage( void ) { }

ErrorPage::~ErrorPage( void ) { }

bool	ErrorPage::inputStr( const std::string &token ) {
	if (path.length() > 0) {
		return (false);
	}
	if (all_of(token.begin(), token.end(), ::isdigit)) {
		codes.push_back(std::atoi(token.c_str()));
		return (true);
	}
	path = token;
	return (true); 
}

bool	ErrorPage::findError( std::string &path_, int status ) {
	for (size_t i = 0; i < codes.size(); i ++) {
		if (status == codes[i]) {
			path_ = path;
			return (true);
		}
	}
	return (false);
}
