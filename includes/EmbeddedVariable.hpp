/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EmbeddedVariable.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 04:11:01 by joshua            #+#    #+#             */
/*   Updated: 2024/11/07 13:55:11 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EMBEDDEDVARIABLE_HPP
# define EMBEDDEDVARIABLE_HPP
# include "Http.hpp"

enum short_form {
	arg_ = 129,
	args,
	query_string,
	content_length,
	content_type,
	document_root,
	document_uri,
	document_url,
	host,
	uri,
	url,
	remote_addr,
	remote_port,
	request_method,
	scheme
};

class EmbeddedVariable {
	public:
		EmbeddedVariable( void );
		EmbeddedVariable( const EmbeddedVariable &src );
		EmbeddedVariable& operator=(const EmbeddedVariable &src);
		~EmbeddedVariable( void );
		static void	shortFormString( std::string &str );
		static void resolveString( std::string &res, const std::string &ref, const Client &client );

	private:
		static const char		*variables[];
		static const uint8_t	shortform[];
}; 

#endif
