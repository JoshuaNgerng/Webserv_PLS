/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 02:31:26 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/28 23:09:48 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP
# include "Const.hpp"
# include "InfoBlock.hpp"
# include <algorithm>

typedef enum auto_index_format {
	none,
	html,
	xml,
	json,
	jsonp
}	format;

class AutoIndex {
public:
	AutoIndex( void );
	AutoIndex( const InfoBlock &src );
	AutoIndex( format f, boolean bt, boolean bs );
	AutoIndex( const AutoIndex &src );
	AutoIndex& operator=( const AutoIndex &src );
	~AutoIndex( void );
	const char	*getExtension( void ) const;
	std::string generateResource( const std::string &dirname );
	std::string generateResource( const char *dirname );

private:
	struct Entry {
		int check;
		std::string fname;
		std::string time;
		uint64_t size;

		Entry(int c, const std::string &f, const std::string &t, uint64_t s)
        	: check(c), fname(f), time(t), size(s) {}
    };

	// Functor to replace the lambda function
	struct EntryComparator {
		bool operator()(const Entry &a, const Entry &b) const {
			if (a.check == b.check) {
				return a.fname < b.fname;
			}
			return a.check > b.check;
		}
	};

	std::vector<Entry> entries;
	int		autoindex_format;
	bool	autoindex_time;
	bool	autoindex_exact_size;

	std::string	root_name;
	size_t		root_len;
	DIR			*dir;

	int			iterDir( std::string &name, std::string &time, uint64_t &size );
	std::string	generateHtml( DIR *dir );
	std::string	generateXml( DIR *dir );
	std::string	generateJson( DIR *dir );

	static const char *template_html;
	static const char *template_xml;
};

#endif