/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 01:46:54 by joshua            #+#    #+#             */
/*   Updated: 2024/09/18 01:31:10 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

Http::Http(const std::string& request) : got_request_line(false), got_content_type(false), got_content_length(false), got_host(false), got_user_agent(false), got_accept(false), got_authorization(false), got_connection(false), got_accept_encoding(false), got_accept_language(false), got_request_headers(false), got_body(false)
{
	std::string::size_type request_line_end = request.find("\r\n");
	std::string::size_type headers_end = request.find("\r\n\r\n");
	if (request_line_end == std::string::npos || headers_end == std::string::npos || request_line_end >= headers_end)
		throw HttpError(incomplete_request_format);
	
	std::string request_line = request.substr(0, request_line_end);
	std::string headers = request.substr(request_line_end + 2, headers_end - (request_line_end + 2));
	std::string body = request.substr(headers_end + 4);

	parse_request_line(request_line);
	parse_request_headers(headers);
	parse_request_body(body);
}

void Http::parse_request_line(const std::string& request)
{
	std::stringstream request_stream(request);
	std::string line;

	if (std::getline(request_stream, line))
	{
		size_t last_non_space = line.find_last_not_of(" \t");
		if (last_non_space != std::string::npos && last_non_space != (line.length() - 1))
			throw HttpError(wrong_request_line_format);

		std::stringstream line_stream(line);
		if (!(line_stream >> method >> uri >> http_version))
			throw HttpError(incomplete_request_line);
		
		validate_method(method);
		validate_uri(uri);
		validate_http_version(http_version);
	
		got_request_line = true;
	}
	else
		throw HttpError(wrong_request_line_format);
}

void Http::validate_method(const std::string& method) const
{
	std::set<std::string> valid_methods;
	valid_methods.insert("GET");
	valid_methods.insert("POST");
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
		// If there are spaces before \r\n, give an error
		size_t last_non_space = line.find_last_not_of(" \t");
		if (last_non_space != std::string::npos && last_non_space != (line.length() - 1))
			throw HttpError(wrong_header_format);
		
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
	validate_headers();
}

void Http::validate_headers()
{
	// Function: State the domain name of the server
	// Validate: Must be present in HTTP/1.1 request.
	if (got_host)
	{
		if (host.empty())
			throw HttpError(invalid_host);

		// validate if the host contain valid domain name or ip
		// Check if the host has a colon (':') and a valid port number after the domain
		std::string::size_type colon_pos = host.find(':');
		if (colon_pos != std::string::npos)
		{
			std::string port = host.substr(colon_pos + 1);
			for (std::string::size_type i = 0; i < port.length(); i++)
			{
				if (!std::isdigit(port[i]))
					throw HttpError(invalid_host);
			}
		}
	}

	// Function: State the size of the request body in bytes
	// Validate: Must be a positive ineteger, if present the body should match this length
	if (got_content_length)
	{
		try
		{
			std::stringstream stream(content_length);
			int len;
			stream >> len;
			if (stream.fail() || len < 0)
				throw HttpError(invalid_content_length);
		}
		catch (const std::exception& e)
		{
			throw HttpError(invalid_content_length);
		}
	}
	
	// Function:: State the media type of the request body
	// validate: Must be a valid MIME type (e.g., "text/html", "application/json")
	if (got_content_type)
	{
		if (content_type.empty() || content_type.find('/') == std::string::npos)
			throw HttpError(invalid_content_type);
	}

	// Function: Gives info about the client that made the request
	// Validate: No strict format, but should not be empty
	if (got_user_agent)
	{
		if (user_agent.empty())
			throw HttpError(invalid_user_agent);
	}

	// Function: State the media types that the client is willing to accept
	// Validate: Must be a valid MIME type or a list of MIME types
	if (got_accept)
	{
		if (accept.empty() || accept.find('/') == std::string::npos)
			throw HttpError(invalid_accept);
	}

	// Function: Store credentials for authetication between the client to the server
	// Validate: Not empty if present, format vary
	if (got_authorization)
	{
		if (authorization.empty())
			throw HttpError(invalid_authorization);
	}

	// Function: Control whether the network connection stays open or close after the request
	// Validate: Common values like "keep-alive" and "close"
	if (got_connection)
	{
		std::set<std::string> valid_values;
		valid_values.insert("keep-alive");
		valid_values.insert("close");
		if (valid_values.find(connection) == valid_values.end())
			throw HttpError(invalid_connection);
	}

	// Function: state the content encoding the client can understand (e.g., gzip, deflate)
	// Validate: should list valid encodign types
	if (got_accept_encoding)
	{
		std::set<std::string> valid_encodings;
		valid_encodings.insert("gzip");
		valid_encodings.insert("deflate");
		valid_encodings.insert("br");
		std::stringstream stream(accept_encoding);
		std::string encoding;
		while (std::getline(stream, encoding, ','))
		{
			trim(encoding);
			if (encoding.empty())
				throw HttpError(invalid_accept_encoding);
			if (valid_encodings.find(encoding) == valid_encodings.end())
				throw HttpError(invalid_accept_encoding);
		}
	}

	// Function: the preferred language for the response
	// Validate: Should list valid language tags
	if (got_accept_language)
	{
		if (accept_language.empty() || accept_language.find('-') == std::string::npos)
			throw HttpError(invalid_accept_language);
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


void Http::parse_request_body(const std::string& body)
{
	std::string trimmed_body = body;
	trimmed_body.erase(0, trimmed_body.find_first_not_of(" \t\r\n"));
	trimmed_body.erase(trimmed_body.find_last_not_of(" \t\r\n") + 1);
	if (got_content_length)
	{
		std::stringstream stream(content_length);
		int len;
		stream >> len;
		if (stream.fail() || len < 0)
			throw HttpError(invalid_content_length);
		// std::cout << body.length() << ", " << len << std::endl;
		if (trimmed_body.length() != static_cast<std::string::size_type>(len))
			throw HttpError(not_matching_body_length);
		this->body = trimmed_body;
		got_body = true;
	}
	else
	{
		// when content-length is not present
		if (method == "POST")
		{
			this->body = trimmed_body; // body is expected for post
			got_body = true;
		}
		else
		{
			// methods like get should not have body when no content length
			if (!body.empty())
				throw HttpError(method_shouldnt_have_body);
		}
	}
}

bool Http::has_body() const
{
	return got_body;
}

std::string Http::get_body() const
{
	if (!has_body())
		return "";
	return body;
}
