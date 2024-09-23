/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reponse.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 00:04:56 by joshua            #+#    #+#             */
/*   Updated: 2024/09/19 00:39:05 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reponse.hpp"

const char * const * Reponse::status_msg = (const char *[]){
	"Continue",
	"Switching Protocols",
	"Processing",
	"Early Hints",
	"OK",
	"Created",
	"Accepted",
	"Non-Authoritative Information",
	"No Content",
	"Reset Content",
	"Partial Content",
	"Multi-Status",
	"Already Reported",
	"IM Used",
	"Multiple choice",
	"Moved Permanently",
	"Found",
	"See Other",
	"Not Modified",
	"Use Proxy",
	"unused",
	"Temporary Redirect",
	"Permanent Redirect",
	"Bad Request",
	"Unauthorized",
	"Payment Required",
	"Forbidden",
	"Not Found",
	"Method Not Allowed",
	"Not Acceptable",
	"Proxy Authentication Required",
	"Request Timeout",
	"Conflict",
	"Gone",
	"Length Required",
	"Precondition Failed",
	"Content Too Large",
	"URI Too Long",
	"Unsupported Media Type",
	"Range Not Satisfiable",
	"Expectation Failed",
	"I'm a teapot",
	"Misdirected Request",
	"Unprocessable Content",
	"Locked",
	"Failed Dependency",
	"Too Early",
	"Upgrade Required",
	"Precondition Required",
	"Too Many Requests",
	"Request Header Fields Too Large",
	"Unavailable For Legal Reasons",
	"Internal Server Error",
	"Not Implemented",
	"Bad Gateway",
	"Service Unavailable",
	"Gateway Timeout",
	"HTTP Version Not Supported",
	"Variant Also Negotiated",
	"Insufficient Storage",
	"Loop Detected",
	"Not Extended",
	"Network Authentication Required",
	NULL
};

const int   *Reponse::status_code = (int []) {
	100, 101, 102, 103,
	200, 201, 202, 203, 204, 205, 206, 207, 208, 226,
	300, 301, 302, 303, 304, 307, 308,
	400, 401, 402, 403, 404, 405, 406, 407, 408, 409,
	410, 411, 412, 413, 414, 415, 416, 417, 418, 421, 422, 423, 424, 425,
	426, 428, 429, 431, 451,
	500, 501, 502, 503,504, 505, 506
};