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

class AutoIndex {
public:
	AutoIndex( void );
	AutoIndex( format f, boolean bt, boolean bs );
	AutoIndex(const AutoIndex &other);
	AutoIndex& operator=(const AutoIndex &other);
	~AutoIndex( void );
	std::string generateResource( const std::string &dirname ) const;
	std::string generateResource( const char *dirname ) const;

private:
	format autoindex_format;
	boolean autoindex_time;
	boolean autoindex_exact_size;
};

#endif