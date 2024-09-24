/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:06:49 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/24 17:35:39 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP
# include "InfoBlock.hpp"

class Location : public InfoBlock {
	public:
		Location( void );
		Location( const std::string &path );
		~Location( void );
		void	reset( void );

	private:
		std::string	path;
		std::string	alias;
		bool		internal;
		bool		is_cgi;
		//limit_except
		
		// std::vector<std::string>	return_add;
		// std::vector<std::string>	allow_methods;
		// std::vector<std::string>	cgi_path;
		// std::vector<std::string>	cgi_ext;

};

#endif