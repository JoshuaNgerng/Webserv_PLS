/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:03:12 by ychng             #+#    #+#             */
/*   Updated: 2024/10/07 20:32:07 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include <iostream>
#include <cassert>

// c++ request_test.cpp srcs/Http.cpp srcs/HttpRequest.cpp -Iincludes/

void valid_get_request();
void extra_space_before_method();
void extra_space_after_method();
void extra_space_after_uri();
void extra_space_after_version();
void extra_space_between_field();
void missing_space_between_method_uri();
void missing_space_between_uri_version();

void invalid_method();
void missing_method();
void invalid_uri();
void missing_uri();
void invalid_version();
void missing_version();
void missing_start_line();

void invalid_header();
void duplicate_headers();

void request_with_multiple_methods();
void request_with_trailing_space();
	
// Erros
void missing_host_header();
void invalid_host_header();
void line_continuation_header();

int main()
{
	valid_get_request();
	extra_space_before_method();
	extra_space_after_method();
	extra_space_after_uri();
	extra_space_after_version();
	extra_space_between_field();
	missing_space_between_method_uri();
	missing_space_between_uri_version();

	invalid_method();
	missing_method();
	invalid_uri();
	missing_uri();
	invalid_version();
	missing_version();
	missing_start_line();

	invalid_header();
	duplicate_headers();
	line_continuation_header();

	request_with_multiple_methods();
	// request_with_trailing_space(); (not sure)
	
	// Erros
	missing_host_header();
	invalid_host_header();
}

void valid_get_request()
{
	HttpRequest request, request1, request2;
    assert(request.addRequest("GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n") != std::string::npos);
    // assert(request.addRequest("GET /index.html HTTP/1.1\r\nHost: example.com\r\nContent-Length: 0\r\n\r\n") != std::string::npos); // with content-legnth
    // assert(request.addRequest("GET /index.html HTTP/1.1\r\nHost: example.com\r\nContent-Type: text/html\r\n\r\n") != std::string::npos); // with content-type
    // assert(request.addRequest("GET /index.html HTTP/1.1\r\nHost: example.com\r\nContent-Length: 0\r\nContent-Type: text/html\r\n\r\n") != std::string::npos); // with both content-length and content-type
    // assert(request.addRequest("GET /index.html HTTP/1.1\r\nHost: example.com\r\nContent-Length: 0\r\nContent-Type: text/html\r\n\r\nThis is a test body.") != std::string::npos); // with both content-length and content-type and body
}

void extra_space_before_method()
{
    HttpRequest request;
    assert(request.addRequest(" GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

void extra_space_after_method()
{
    HttpRequest request;
    assert(request.addRequest("GET  /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

void extra_space_after_uri()
{
    HttpRequest request;
    assert(request.addRequest("GET /index.html  HTTP/1.1\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

void extra_space_after_version()
{
    HttpRequest request;
    assert(request.addRequest("GET /index.html HTTP/1.1 \r\nHost: example.com\r\n\r\n") == std::string::npos);
}

void extra_space_between_field()
{
    HttpRequest request;
    assert(request.addRequest("GET   /index.html  HTTP/1.1\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

void missing_space_between_method_uri()
{
    HttpRequest request;
    assert(request.addRequest("GET/index.html HTTP/1.1\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

void missing_space_between_uri_version()
{
    HttpRequest request;
    assert(request.addRequest("GET /index.htmlHTTP/1.1\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

void invalid_method()
{
    HttpRequest request;
    assert(request.addRequest("INVALID /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

void missing_method()
{
	HttpRequest request;
	assert(request.addRequest("/index.html HTTP/1.1\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

// invalid characters
void invalid_uri()
{
    HttpRequest request;
    assert(request.addRequest("GET /index.html # HTTP/1.1\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

void missing_uri()
{
    HttpRequest request;
    assert(request.addRequest("GET  HTTP/1.1\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

void invalid_version()
{
    HttpRequest request, request1;
    assert(request.addRequest("GET /index.html HTTP/2.0\r\nHost: example.com\r\n\r\n") == std::string::npos);
    assert(request1.addRequest("GET /index.html HTTP/1.10\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

void missing_version()
{
    HttpRequest request;
    assert(request.addRequest("GET /index.html\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

void missing_start_line()
{
	HttpRequest request;
    assert(request.addRequest("\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

// invalid characters
void invalid_header()
{
    HttpRequest request;
    assert(request.addRequest("GET /index.html HTTP/1.1\r\nHo@st: example.com\r\n\r\n") == std::string::npos);
}

// HTTP/1.1 allow, value is Content-Length: 5 This is continued
// HTTP/2 doesn't allow
void line_continuation_header()
{
	HttpRequest request;
    assert(request.addRequest("GET /index.html HTTP/1.1\r\nHost: example.com\r\nContent-Length: 5\r\n This is continued\r\n\r\n") == std::string::npos);
}

void duplicate_headers()
{
    HttpRequest request;
    assert(request.addRequest("GET /index.html HTTP/1.1\r\nHost: example.com\r\nHost: another.com\r\n\r\n") == std::string::npos);
}

void request_with_multiple_methods()
{
    HttpRequest request, request1;
    assert(request.addRequest("GET POST /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n") == std::string::npos);
    assert(request1.addRequest("GeT pOsT /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n") == std::string::npos);
}

// std::stringstream ss("Host: example.com         \r\n");
// std::string example;
// getline(ss,example);
// std::string field = example.substr(4);
// std::string buffer = example.substr(5);
// std::cout << buffer << "m" << "\n";
void request_with_trailing_space()
{
	HttpRequest request;
	std::cout << request.addRequest("GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n") << "\n";
    // assert(request.addRequest("GET /index.html HTTP/1.1\r\nHost: example.com   \r\n\r\n") == std::string::npos);
}

// Errors
void missing_host_header()
{
    HttpRequest request;
    assert(request.addRequest("GET /index.html HTTP/1.1\r\n\r\n") == std::string::npos);
}

void invalid_host_header()
{
    HttpRequest request;
    assert(request.addRequest("GET /index.html HTTP/1.1\r\nHost: :example.com\r\n\r\n") == std::string::npos);
}
