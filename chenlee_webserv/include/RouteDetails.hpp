/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteDetails.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 03:10:02 by jatan             #+#    #+#             */
/*   Updated: 2024/08/10 19:32:08 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTEDETAILS_HPP
#define ROUTEDETAILS_HPP

#include <string>

class RouteDetails
{
public:
	RouteDetails();
	RouteDetails(std::string route);
	RouteDetails(bool autoindex, std::string index, std::string root, int allowedMethods);
	RouteDetails(bool autoindex, std::string index, std::string root, int allowedMethods, std::string cgiPass);
	~RouteDetails();

	std::string route;
	std::string redirection;
	bool autoindex;
	std::string index;
	std::string root;
	std::string cgiPass;
	int allowedMethods;
};

#endif
