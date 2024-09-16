/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpError.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 01:59:21 by ychng             #+#    #+#             */
/*   Updated: 2024/09/17 03:12:58 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpError.hpp"
#include <iostream>

HttpError::HttpError() {}
HttpError::HttpError(const HttpError& src) { msg = src.msg; }
HttpError::~HttpError() {}
HttpError& HttpError::operator=(const HttpError& src) { if (this != &src) msg = src.msg; return *this; }
const char*	HttpError::what() const throw() { return msg.c_str(); }

HttpError::HttpError(int val)
{
	msgInit(val);
}

void HttpError::msgInit(int val)
{
	msg = "HttpError: ";
	switch (val)
	{
		case missing_request_line:
			msg += "Missing request line";
			break ;
		case incomplete_request_line:
			msg += "incomplete request line";
			break ;
		case wrong_request_method:
			msg += "Wrong request method";
			break ;
		case wrong_request_uri:
			msg += "Wrong request uri";
			break ;
		case wrong_request_http_version:
			msg += "Wrong request http version";
			break ;
		default:
			msg += "Unkwown error";
	}
}
