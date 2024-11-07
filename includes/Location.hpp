/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:06:49 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/07 10:41:14 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP
# include "InfoBlock.hpp"
# include "LimitExcept.hpp"

class Location : public InfoBlock {
	typedef std::map<std::string, std::string>				CgiMapping;
	typedef std::map<std::string, std::string>::iterator	CgiIter;
	public:
		Location( void );
		Location( const Location &src );
		Location& operator=( const Location &src );
		~Location( void );
		void	addPath( const std::string &path );
		void	addAlias( const std::string &path );
		void	setInternal( void );
		void	addCgiMapping( const std::string &ext );
		void	addCgiMapping( const std::string &ext, const std::string &interpret );
		void	reset( void );

		const std::string&	getLocationPath( void ) const;
		void	routingClient( Client &client ) const;
		void	defaultSetting( void );
		void	defaultSetting( const InfoBlock &ref );

	private:
		std::string	path;
		std::string	alias;
		bool		internal;
		bool		cgi_enabled;
		CgiMapping	cgi_mapping;
		LimitExcept	limit_except;

		std::pair<int, std::string>	return_;
};

#endif
