/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EmbeddedVariable.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 04:11:01 by joshua            #+#    #+#             */
/*   Updated: 2024/09/27 18:09:36 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EMBEDDEDVARIABLE_HPP
# define EMBEDDEDVARIABLE_HPP
# include "Http.hpp"

enum short_form {
	arg_name = 129,
	args,
	query_string,
	content_length,
	content_type,
	document_root,
	document_url,
	host,
	url,
	remote_addr,
	remote_port,
	request_method,
	scheme,
	status
};

class EmbeddedVariable {
	public:
		~EmbeddedVariable( void );
		static void	shortFormString( std::string &str );
		// static void	shortFormStringAllow( std::string &str, int8_t *allow );
		// static void	shortFormStringDeny( std::string &str, int8_t *deny );
		static void resolveString( std::string &str, ... );

	private:
		static const char	**variables;
		static const uint8_t	*shortform;
		EmbeddedVariable( void );
		EmbeddedVariable( const EmbeddedVariable &src );
		EmbeddedVariable&	operator=( const EmbeddedVariable &src );
};

#endif