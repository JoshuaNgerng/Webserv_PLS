/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 01:46:54 by joshua            #+#    #+#             */
/*   Updated: 2024/09/17 03:24:07 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

Http::Http(const std::string& request) : got_request_line(false)
{
	parse_request_line(request);
}

void Http::parse_request_line(const std::string& request)
{
	std::stringstream request_stream(request);
	std::string line;

	if (std::getline(request_stream, line))
	{
		std::stringstream line_stream(line);
		if (!(line_stream >> method >> uri >> http_version))
			throw HttpError(incomplete_request_line);
		
		validate_method(method);
		validate_uri(uri);
		validate_http_version(http_version);
	
		got_request_line = true;
	}
	else
		throw HttpError(missing_request_line);
}

void Http::validate_method(const std::string& method) const
{
	const std::set<std::string> valid_methods = {"GET", "POST"};
	if (valid_methods.find(method) == valid_methods.end())
		throw HttpError(wrong_request_method);
}

void Http::validate_uri(const std::string& uri) const
{
	if (uri.empty() || uri[0] != '/')
		throw HttpError(wrong_request_uri);
}

void Http::validate_http_version(const std::string& http_version) const
{
	if (http_version != "HTTP/1.1")
		throw HttpError(wrong_request_http_version);
}

bool Http::has_request_line() const
{
	return got_request_line;
}

void Http::print_request_line() const
{
	if (got_request_line)
	{
		std::cout << "Method: " << method << "\n"
				  << "URI: " << uri << "\n"
				  << "HTTP Version: " << http_version << "\n";
	}
	else
		std::cout << "Invalid request line.\n";
}
