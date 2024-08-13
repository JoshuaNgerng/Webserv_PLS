/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 01:51:31 by chenlee           #+#    #+#             */
/*   Updated: 2024/06/18 17:05:10 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <sys/stat.h>

#include "Request.hpp"
#include "Response.hpp"
#include "RouteDetails.hpp"

class RequestHandler
{
public:
    virtual ~RequestHandler(){};

    /**
     *
     * Virtual function to handle request.
     *
     * @param req The request object containing the client's request information.
     * @param res The response object used to send the response back to the client.
     * @param root The root directory path to find the resource.
     *
     * @return * true if the request was handled, false if the request was not handled.
     */
    virtual bool handleRequest(const Request &request, Response &response, RouteDetails &routeDetail, const std::string &fullPath) = 0;

    /**
     *
     * Virtual function to check if should handle request.
     *
     * @param req The request object containing the client's request information.
     * @param res The response object used to send the response back to the client.
     * @param root The root directory path to find the resource.
     *
     * @return * true if the request should handle, false if the request should not be handled.
     */
    virtual bool checkIfHandle(const Request &request, RouteDetails &routeDetail, const std::string &fullPath) = 0;
};

#endif
