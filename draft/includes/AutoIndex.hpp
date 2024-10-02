/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 02:31:26 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/02 04:57:06 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP
# include "Const.hpp"

enum auto_index_format {
	undefined,
	html,
	xml,
	json,
	jsonp
};

class AutoIndex
{
	typedef enum auto_index_format format;
	public:
		AutoIndex( void );
		AutoIndex( format f, boolean bt, boolean bs );
		~AutoIndex( void );
		std::string	generateResource( const std::string &dirname ) const;
		std::string	generateResource( const char *dirname ) const;

	private:
		std::string	generateHtml( DIR *dir ) const;
		std::string	generateXml( DIR *dir ) const;
		std::string	generateJson( DIR *dir ) const;

		format	autoindex_format;
		bool	autoindex_time;
		bool	autoindex_exact_size;
};

#endif