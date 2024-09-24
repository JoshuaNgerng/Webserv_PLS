/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestError.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 19:06:10 by ychng             #+#    #+#             */
/*   Updated: 2024/09/24 19:18:34 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestError.hpp"

HttpRequestError::HttpRequestError() {}
HttpRequestError::~HttpRequestError() {}
HttpRequestError::HttpRequestError(const HttpRequestError& src) { (void)src; }
HttpRequestError& HttpRequestError::operator=(const HttpRequestError& src) { if (this != &src) (void)src; return *this; }

HttpRequestError::HttpRequestError(int val) { msgInit(val); }
const char* HttpRequestError::what() const throw() { return msg.c_str(); }

void HttpRequestError::msgInit(int val)
{
	msg = "HttpRequestError: ";
    switch (val)
    {
        case missing_request_line:
            msg += "Missing request line.";
            break;
        case trailing_spaces:
            msg += "Trailing spaces detected.";
            break;
        case incomplete_request_line:
            msg += "Incomplete request line.";
            break;
        case invalid_method:
            msg += "Invalid HTTP method.";
            break;
        case invalid_uri:
            msg += "Invalid URI.";
            break;
        case invalid_version:
            msg += "Invalid HTTP version.";
            break;
        case invalid_content_type:
            msg += "Invalid Content-Type.";
            break;
        case invalid_content_length:
            msg += "Invalid Content-Length.";
            break;
        case invalid_host:
            msg += "Invalid Host header.";
            break;
        case invalid_user_agent:
            msg += "Invalid User-Agent header.";
            break;
        case invalid_accept:
            msg += "Invalid Accept header.";
            break;
        case incomplete_header:
            msg += "Incomplete header line.";
            break;
        case invalid_body_length:
            msg += "Invalid body length.";
            break;
        case invalid_body:
            msg += "Invalid body for this request.";
            break;
        default:
            msg += "Unknown error.";
            break;
    }
}
