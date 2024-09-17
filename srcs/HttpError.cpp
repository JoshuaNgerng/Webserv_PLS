/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpError.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 01:59:21 by ychng             #+#    #+#             */
/*   Updated: 2024/09/18 00:39:00 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpError.hpp"
#include <iostream>

HttpError::HttpError() {}
HttpError::HttpError(const HttpError& src) { msg = src.msg; }
HttpError::~HttpError() throw() {}
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
		case incomplete_request_format:
			msg += "Incomplete request format";
			break ;
		case incomplete_request_line:
			msg += "Incomplete request line";
			break ;
		case wrong_request_line_format:
			msg += "Wrong request line format";
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
		case incomplete_header_format:
			msg += "Incomplete header format";
			break ;
		case wrong_header_format:
			msg += "Wrong header format";
			break ;
		case invalid_host:
			msg += "Invalid host";
			break ;
		case invalid_content_length:
			msg += "invalid content length";
			break ;
		case invalid_content_type:
			msg += "Invalid content type";
			break ;
		case invalid_user_agent:
			msg += "Invalid user agent";
			break ;
		case invalid_accept:
			msg += "Invalid accept";
			break ;
		case invalid_authorization:
			msg += "invalid authorization";
			break ;
		case invalid_connection:
			msg += "invalid connection";
			break ;
		case invalid_accept_encoding:
			msg += "invalid accept encoding";
			break ;
		case invalid_accept_language:
			msg += "Invalid accept language";
			break ;
		default:
			msg += "Unkwown error";
	}
}
