/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:06:49 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/08 21:09:05 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP
# include "InfoBlock.hpp"
# include "LimitExcept.hpp"

class Location : public InfoBlock {
	public:
		Location( void );
		Location( const Location &src );
		Location& operator=( const Location &src );
		~Location( void );
		void	addPath( const std::string &path );
		void	addAlias( const std::string &path );
		void	addReturn( int code );
		void	addReturn( const std::string &add );
		void	addReturn( int code, const std::string &add );
		void	setInternal( void );
		void	reset( void );

		void	routingClient( Client &client ) const;
		void	defaultSetting( void );
		void	defaultSetting( const InfoBlock &ref );
		int		checkReturnCode( void ) const;

		const std::string&	checkReturnUri( void ) const;
		const std::string&	getLocationPath( void ) const;

	private:
		std::string	path;
		std::string	alias;
		bool		internal;

		std::pair<int, std::string>	return_;

		void	routeClientReturn( Client &client ) const;
};

#endif
