/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:06:49 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/23 13:06:33 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP
# include "InfoBlock.hpp"
# include "LimitExcept.hpp"

class Location : public InfoBlock {
	typedef std::map<std::string, std::string> CgiMapping;
	public:
		Location( void );
		Location( const Location &src );
		Location& operator=( const Location &src );
		~Location( void );
		void	addPath( const std::string &path );
		void	addAlias( const std::string &path );
		void	setInternal( void );
		void	reset( void );

		const std::string&	getLocationPath( void ) const;
		void	matchUri( Client &client, bool autoindex_ ) const;

	private:
		std::string	path;
		std::string	alias;
		bool		internal;

		CgiMapping	cgi_mapping;
		LimitExcept	limit_except;

		std::pair<int, std::string>	return_;

		bool		is_cgi;

};

#endif
