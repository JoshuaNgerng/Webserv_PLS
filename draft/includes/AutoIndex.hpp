/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 02:31:26 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/08 03:07:21 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP
# include "Const.hpp"

typedef enum auto_index_format {
	undefined,
	html,
	xml,
	json,
	jsonp
}	format;

class AutoIndex {
public:
	AutoIndex( void );
	AutoIndex( format f, boolean bt, boolean bs );
	AutoIndex( const AutoIndex &src );
	AutoIndex& operator=(const AutoIndex &src);
	~AutoIndex( void );
	std::string generateResource( const std::string &dirname ) const;
	std::string generateResource( const char *dirname ) const;

private:
	format	autoindex_format;
	bool	autoindex_time;
	bool	autoindex_exact_size;

	std::string	generateHtml( DIR *dir, const char *dirname ) const;
	std::string	generateXml( DIR *dir ) const;
	std::string	generateJson( DIR *dir ) const;

	static const char *template_html;
	static const char *template_xml;
};

#endif