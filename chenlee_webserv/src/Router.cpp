/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:58:14 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/10 21:12:04 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"
#include "ServerBlock.hpp"

#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

std::string getCwd()
{
	char cwd[PATH_MAX];
	std::string projectDir;
#ifdef _WIN32
	if (GetCurrentDirectory(PATH_MAX, cwd) != 0)
	{
		projectDir = cwd;
	}
	else
	{
		printf("Failed to get project directory: %lu\n", GetLastError());
	}
#else
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		projectDir = cwd;
	}
	else
	{
		perror("Failed to get project directory");
	}
#endif

	// Append "/" to the end of projectDir if it's not already there
	if (!projectDir.empty())
	{
#ifdef _WIN32
		if (projectDir.back() != '\\')
			projectDir += "\\";
#else
		if (projectDir.back() != '/')
			projectDir += "/";
#endif
	}
	return (projectDir);
}

Router::Router() : _logger(Logger("Router"))
{
	_projectDir = getCwd();
}

Router::Router(std::string projectDir) : _projectDir(projectDir), _logger(Logger("Router"))
{
	_projectDir = getCwd();
}
Router::Router(ServerBlock *serverBlock) : _logger(Logger("Router")), _serverBlock(serverBlock)
{
	_projectDir = getCwd();
}

Router::~Router()
{
	for (std::map<std::string, RouteDetails *>::iterator it = this->_routeTable.begin(); it != this->_routeTable.end(); it++)
	{
		delete it->second;
	}
	this->_routeTable.clear();
}

void Router::addRoute(const std::string &path, RouteDetails *routeDetail)
{
	this->_routeTable[path] = routeDetail;
}

void Router::assignHandlers(requestHandlerVec &handlers)
{
	this->_requestHandlers = &handlers;
}

bool checkAllowedMethods(std::string reqMethods, int allowedMethods)
{
	if (reqMethods == "GET")
	{
		return allowedMethods & GET;
	}
	else if (reqMethods == "POST")
	{
		return allowedMethods & POST;
	}
	else if (reqMethods == "DELETE")
	{
		return allowedMethods & DELETE;
	}
	return false;
}

RouteDetails *Router::getRouteDetails(const std::string &path, const routeTableMap &routeTable)
{
	RouteDetails *ret;
	for (routeTableMap::const_iterator it = routeTable.begin(); it != routeTable.end(); ++it)
	{
		if (path.find(it->first) != std::string::npos)
		{
			ret = it->second;
			if (!ret->cgiPass.empty())
				return ret;
		}
	}
	return ret;
}

void Router::routeRequest(const Request &request, Response &response)
{
	std::cout << request.getBody().length() << " " << this->_serverBlock->getBodyLimit() << std::endl;
	if (request.getBody().length() > static_cast<std::string::size_type>(this->_serverBlock->getBodyLimit()))
	{
		response.errorResponse(413, "Request Entity Too Large");
		return;
	}

	RouteDetails *routeDetail = getRouteDetails(request.getUri(), this->_routeTable);
	if (routeDetail)
	{
		this->_logger.log("Handling route detail: " + routeDetail->route);
		if (routeDetail->redirection.empty() && checkAllowedMethods(request.getMethod(), routeDetail->allowedMethods) == false)
		{
			response.errorResponse(405, "Method not allowed");
			return;
		}

		std::string fullPath = this->_projectDir + routeDetail->root + request.getResource();
		for (requestHandlerVecIt it = this->_requestHandlers->begin(); it != this->_requestHandlers->end(); it++)
		{
			if ((*it)->checkIfHandle(request, *routeDetail, fullPath))
			{
				if ((*it)->handleRequest(request, response, *routeDetail, fullPath))
					return;
			}
		}
		response.errorResponse(404, "no handler found");
	}
	else
	{
		response.errorResponse(404, "route not found in route table");
	}
}

void Router::display(void) const
{
}
