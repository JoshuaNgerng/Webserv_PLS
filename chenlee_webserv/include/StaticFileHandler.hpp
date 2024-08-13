/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:54:29 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/10 21:06:37 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATICFILEHANDLER_HPP
#define STATICFILEHANDLER_HPP

// #define CUSTOM_PATH_MAX 32767

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <ctime>

#include "HttpMethods.h"
#include "RequestHandler.hpp"
#include "Utility.hpp"

class StaticFileHandler : public RequestHandler
{
public:
    StaticFileHandler();
    ~StaticFileHandler();
    bool handleRequest(const Request &request, Response &response, RouteDetails &routeDetail, const std::string &fullPath);
    bool checkIfHandle(const Request &request, RouteDetails &routeDetail, const std::string &fullPath);
    
};

#endif
