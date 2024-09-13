/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: folim <folim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:17:28 by folim             #+#    #+#             */
/*   Updated: 2024/09/13 17:17:30 by folim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATION_H__
    #define __LOCATION_H__
#include "ServerBlock.hpp"

class Location
{
	public:
		Location(const std::string &path);
		Location();

    // private: // ignore private types for now
		std::string					path;
		
	// reason why I put those one method directives as vector is to
	// accomodate the pushDirective function. I don't have to copy again for string..
		std::vector<std::string>	root;
		std::vector<std::string>	autoindex;
		std::vector<std::string>	index;
		std::vector<std::string>	return_add;
		std::vector<std::string>	alias;


		std::vector<std::string>	allow_methods;
		std::vector<std::string>	cgi_path;
		std::vector<std::string>	cgi_ext;

};

#endif