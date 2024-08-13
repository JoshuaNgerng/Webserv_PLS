/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:45:20 by jatan             #+#    #+#             */
/*   Updated: 2024/05/17 17:14:35 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "RequestHandler.hpp"
#include "Utility.hpp"
#include <unistd.h>
#include <cstring>
#include <fcntl.h>

#define CHUNK_SIZE (4 * 1024)

class CGIHandler : public RequestHandler
{
public:
	CGIHandler();
	// CGIHandler( CGIHandler const & src );
	// CGIHandler & operator=( CGIHandler const & rhs );
	~CGIHandler();

	/** Handles the request for autoindexing.
	 *
	 * This function is responsible for handling the request for autoindexing. It takes in the request object `req`, the response object `res`, and the root directory path `root`. It performs the necessary operations to generate the autoindex page and store the result in response.
	 *
	 * @param req The request object containing the client's request information.
	 * @param res The response object used to send the response back to the client.
	 * @param root The root directory path to find the resource.
	 *
	 * @return * true if the request was handled, false if the request was not handled.
	 */
	bool handleRequest(const Request &request, Response &response, RouteDetails &routeDetails, const std::string &fullPath);
	bool checkIfHandle(const Request &request, RouteDetails &routeDetails, const std::string &fullPath);

private:
	std::string loadHtmlTemplate(void);

	Logger _logger;
};

#endif /* ****************************************************** CGIHANDLER_HPP */
