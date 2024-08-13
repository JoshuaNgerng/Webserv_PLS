/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectionHandler.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 15:41:04 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/10 16:49:11 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectionHandler.hpp"

RedirectionHandler::RedirectionHandler() : _logger(Logger("RedirectionHandler"))
{
}

bool RedirectionHandler::checkIfHandle(const Request &request, RouteDetails &routeDetail, const std::string &fullPath)
{
	(void)request;
	(void)fullPath;
	std::cout << "?? " << routeDetail.redirection << std::endl;
	if (routeDetail.redirection.empty())
	{
		this->_logger.info("No redirection set");
		return false;
	}
	return true;
}

bool RedirectionHandler::handleRequest(const Request &request, Response &response, RouteDetails &routeDetail, const std::string &fullPath)
{
	(void)fullPath;
	(void)request;
	response.setStatusCode(307);
	response.addHeader("Location", routeDetail.redirection);
	response.setBody("");
	return true;
}
