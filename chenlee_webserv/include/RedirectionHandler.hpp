/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectionHandler.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 15:21:38 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/10 21:07:48 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONHANDLER_HPP
#define REDIRECTIONHANDLER_HPP

#include "HttpMethods.h"
#include "RequestHandler.hpp"
#include "Utility.hpp"

class RedirectionHandler : public RequestHandler
{
private:
	Logger _logger;

public:
	RedirectionHandler();
	virtual ~RedirectionHandler() {}
	bool checkIfHandle(const Request &request, RouteDetails &routeDetail, const std::string &fullPath);
	bool handleRequest(const Request &request, Response &response, RouteDetails &routeDetail, const std::string &fullPath);
};

#endif
