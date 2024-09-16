/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 01:46:54 by joshua            #+#    #+#             */
/*   Updated: 2024/09/17 05:30:12 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

Http::Http(const std::string& request) : got_request_line(false), got_content_type(false), got_content_length(false), got_host(false), got_user_agent(false), got_accept(false), got_authorization(false), got_connection(false), got_accept_encoding(false), got_accept_language(false), got_request_headers(false)
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

void Http::parse_request_headers(const std::string& headers)
{
	std::stringstream stream(headers);
	std::string line;
	while (std::getline(stream, line))
	{
		if (line.empty())
		{
			got_request_headers = true;
			break ;
		}
		// Process each header line (simple example)
		std::string name;
		std::string value;
		std::stringstream header_stream(line);
		if (std::getline(header_stream, name, ':') && \
			std::getline(header_stream, value))
		{
			trim(name);
			trim(value);
			to_lowercase(name);
			if (name == "content-type")
			{
				got_content_type = true;
				content_type = value;
			}
			else if (name == "content-length")
			{
				got_content_length = true;
				content_length = value;
			}
			else if (name == "host")
			{
				got_host = true;
				host = value;
			}
			else if (name == "user-agent")
			{
				got_user_agent = true;
				user_agent = value;
			}
			else if (name == "accept")
			{
				got_accept = true;
				accept = value;
			}
			else if (name == "authorization")
			{
				got_authorization = true;
				authorization = value;
			}
			else if (name == "connection")
			{
				got_connection = true;
				connection = value;
			}
			else if (name == "accept-encoding")
			{
				got_accept_encoding = true;
				accept_encoding = value;
			}
			else if (name == "accept-language")
			{
				got_accept_language = true;
				accept_language = value;
			}
		}
		else
			throw HttpError(incomplete_header_format);
	}
}

void Http::trim(std::string& str)
{
	str.erase(0, str.find_first_not_of(" \t"));
	str.erase(str.find_last_not_of(" \t") + 1);
}

void Http::to_lowercase(std::string& str)
{
	if (!str.empty())
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

bool Http::has_content_type() const { return got_content_type; }
bool Http::has_content_length() const { return got_content_length; }
bool Http::has_host() const { return got_host; }
bool Http::has_user_agent() const { return got_user_agent; }
bool Http::has_accept() const { return got_accept; }
bool Http::has_authorization() const { return got_authorization; }
bool Http::has_connection() const { return got_connection; }
bool Http::has_accept_encoding() const { return got_accept_encoding; }
bool Http::has_accept_language() const { return got_accept_language; }

std::string Http::get_content_type() const { return content_type; }
std::string Http::get_content_length() const { return content_length; }
std::string Http::get_host() const { return host; }
std::string Http::get_user_agent() const { return user_agent; }
std::string Http::get_accept() const { return accept; }
std::string Http::get_authorization() const { return authorization; }
std::string Http::get_connection() const { return connection; }
std::string Http::get_accept_encoding() const { return accept_encoding; }
std::string Http::get_accept_language() const { return accept_language; }