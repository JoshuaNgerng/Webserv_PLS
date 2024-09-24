/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:08:42 by ychng             #+#    #+#             */
/*   Updated: 2024/09/24 18:10:56y ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include <set>
#include <algorithm>

HttpRequest::HttpRequest()
		: request_line_exists(false),
		  header_exists(false),
		  body_exists(false),
		  content_type_exist(false),
		  content_length_exist(false),
          host_exists(false),
          user_agent_exists(false),
          accept_exists(false) {}
HttpRequest::~HttpRequest() {}
HttpRequest::HttpRequest(const HttpRequest& src) { (void)src; }
HttpRequest& HttpRequest::operator=(const HttpRequest& src) { if (this != &src) (void)src; return *this; }

// Getters
const std::string& HttpRequest::getMethod() const { return method; }
const std::string& HttpRequest::getUri() const { return uri; }
const std::string& HttpRequest::getVersion() const { return version; }
const std::string& HttpRequest::getRequestLine() const { return request_line; }
const std::string& HttpRequest::getContentType() const { return content_type; }
const std::string& HttpRequest::getContentLength() const { return content_length; }
const std::string& HttpRequest::getHost() const { return host; }
const std::string& HttpRequest::getUserAgent() const { return user_agent; }
const std::string& HttpRequest::getAccept() const { return accept; }
const std::string& HttpRequest::getBody() const { return body; }

// Field existence checker
bool HttpRequest::hasField(Field field) const
{
	switch (field)
	{
		case REQUEST_LINE: return request_line_exists;
		case HEADER: return header_exists;
		case BODY: return body_exists;
		case CONTENT_TYPE: return content_type_exist;
        case CONTENT_LENGTH: return content_length_exist;
        case HOST: return host_exists;
        case USER_AGENT: return user_agent_exists;
        case ACCEPT: return accept_exists;
		default: return false;
	}
}

bool HttpRequest::isValidMethod(const std::string& method) const
{
	std::set<std::string> valid_methods;
	valid_methods.insert("GET");
	valid_methods.insert("POST");
	valid_methods.insert("DELETE");
	return valid_methods.find(method) != valid_methods.end();
}

bool HttpRequest::isValidUri(const std::string& uri) const
{
	return !uri.empty() && uri[0] == '/'; // URI must start with '/'
}

bool HttpRequest::isValidVersion(const std::string& version) const
{
	return version == "HTTP/1.1"; // Only HTTP/1.1 is valid
}

void HttpRequest::trimSpacesAround(std::string& str) const
{
	size_t first = str.find_first_not_of(" \t");
	size_t last = str.find_last_not_of(" \t");
	if (first == std::string::npos)
		str.clear();
	else
		str = str.substr(first, (last - first + 1));
}

void HttpRequest::toLowercase(std::string& str) const
{
	if (!str.empty())
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

bool HttpRequest::parseRequestLine(const std::string& request_line)
{
	std::stringstream stream(request_line);
	std::string first_line;

	// Read the request line for validation
	if (!std::getline(stream, first_line) || first_line.empty())
		throw HttpRequestError(missing_request_line);

	// Validate the request line format (check for no trailing spaces)
	size_t last_non_space = first_line.find_last_not_of(" \t");
	if (last_non_space != std::string::npos && last_non_space != (first_line.length() - 1))
		throw HttpRequestError(trailing_spaces);

	// Use >> to extract method, URI, and version
	if (!(stream >> method >> uri >> version))
		throw HttpRequestError(incomplete_request_line);

	this->request_line = first_line;

	// Validate method, URI, and version
	if (!isValidMethod(method))
		throw HttpRequestError(invalid_method);
	if (!isValidUri(uri))
		throw HttpRequestError(invalid_uri);
	if (!isValidVersion(version))
		throw HttpRequestError(invalid_version);
	request_line_exists = true;
	return request_line_exists;
}

bool HttpRequest::isValidHeaders() const
{
	if (hasField(CONTENT_TYPE))
	{
		if (content_type.empty() || content_type.find('/') == std::string::npos) // Must have a '/'
			throw HttpRequestError(invalid_content_type);
	}
	if (hasField(CONTENT_LENGTH))
	{
		try
		{
			std::stringstream stream(content_length);
			int len;
			stream >> len; // Try to convert content_length to an int
			if (stream.fail() || len < 0) // Must be a positive number
				throw HttpRequestError(invalid_content_length);
		}
		catch (const std::exception& e) // Handle conversion error
		{
			throw HttpRequestError(invalid_content_length);
		}
	}
	if (hasField(HOST))
	{
		if (host.empty()) // Host cannot be empty
			throw (HttpRequestError(invalid_host));

		// Check if the host has a port number
		std::string::size_type delim_pos = host.find(':');
		if (delim_pos != std::string::npos)
		{
			std::string port = host.substr(delim_pos + 1);
			// Check if the port is digits only
			for (std::string::size_type i = 0; i < port.length(); i++)
			{
				if (!std::isdigit(port[i]))
					throw HttpRequestError(invalid_host);
			}
		}
	}
	if (hasField(USER_AGENT))
	{
		if (user_agent.empty()) // User agent cannot be empty
			throw HttpRequestError(invalid_user_agent);
	}
	if (hasField(ACCEPT))
	{
		if (accept.empty() || accept.find('/') == std::string::npos) // Must have '/'
			throw HttpRequestError(invalid_accept);
	}
	return true;
}

bool HttpRequest::parseHeaders(const std::string &header_lines)
{
	std::stringstream stream(header_lines);
	std::string each_line;

	while (std::getline(stream, each_line))
	{
		if (each_line.empty())
			break ;

		// Validate the headers line format (check for no spaces before \r\n)
		size_t last_non_space = each_line.find_last_not_of(" \t");
		if (last_non_space != std::string::npos && last_non_space != (each_line.length() - 1))
			throw HttpRequestError(trailing_spaces);
		
		// Split the line into key and value
		std::string key;
		std::string value;
		std::stringstream header(each_line);
		if (std::getline(header, key, ':') && std::getline(header, value))
		{
			trimSpacesAround(key);
			trimSpacesAround(value);
			toLowercase(key);
		
			// Set the specific headers
			if (key == "content-type") {
				content_type = value;
				content_length_exist = true;
			} else if (key == "content_length") {
				content_length = value;
				content_length_exist = true;
			} else if (key == "host") {
				host = value;
				host_exists = true;
			} else if (key == "user_agent") {
				user_agent = value;
				user_agent_exists = true;
			} else if (key == "accept") {
				accept = value;
				accept_exists = true;
			}
		}
		else throw HttpRequestError(incomplete_header);
	}
	header_exists = isValidHeaders();
    return header_exists; // Headers are parsed successfully
}

bool HttpRequest::parseBody(const std::string &body_line)
{
	// Trim leading and trailing spaces
	std::string body_content = body_line;
	body_content.erase(0, body_content.find_first_not_of(" \t\r\n"));
	body_content.erase(body_content.find_last_not_of(" \t\r\n") + 1);

	// check if Content-Length exist
	if (hasField(CONTENT_LENGTH))
	{
		int expected_length = atoi(content_length.c_str());
		if (expected_length < 0 && body_content.length() != static_cast<size_t>(expected_length))
			throw HttpRequestError(invalid_body_length);
		body = body_content;
		body_exists = true;
	} else {
		if (method == "POST")
		{
			body = body_content;
			body_exists = true;
		}
		else
		{
			// Check if no body exist for other method (e.g, GET)
			if (!body_line.empty())
				throw HttpRequestError(invalid_body);
		}
	}
    return true;
}

void HttpRequest::addRequestHeader(const std::string &header_line)
{
	if (header_line.empty())
		return ;

	// Validate the headers line format (check for no spaces before \r\n)
	size_t last_non_space = header_line.find_last_not_of(" \t");
	if (last_non_space != std::string::npos && last_non_space != (header_line.length() - 1))
		throw HttpRequestError(trailing_spaces);
		
	// Split the line into key and value
	std::string key;
	std::string value;
	std::stringstream header(header_line);
	if (std::getline(header, key, ':') && std::getline(header, value))
	{
		trimSpacesAround(key);
		trimSpacesAround(value);
		toLowercase(key);
	
		// Set the specific headers
		if (key == "content-type") {
			content_type = value;
			content_type_exist = true;
		} else if (key == "content-length") {
			content_length = value;
			content_length_exist = true;
		} else if (key == "host") {
			host = value;
			host_exists = true;
		} else if (key == "user-agent") {
			user_agent = value;
			user_agent_exists = true;
		} else if (key == "accept") {
			accept = value;
			accept_exists = true;
		}
	}
	else throw HttpRequestError(incomplete_header);
	header_exists = isValidHeaders();
}

void HttpRequest::addRequestBody(const std::string& body_line)
{
	// Trim leading and trailing spaces
	std::string body_content = body_line;
	body_content.erase(0, body_content.find_first_not_of(" \t\r\n"));
	body_content.erase(body_content.find_last_not_of(" \t\r\n") + 1);

	// check if Content-Length exist
	if (hasField(CONTENT_LENGTH))
	{
		int expected_length = atoi(content_length.c_str());
		if (expected_length < 0 && body_content.length() != static_cast<size_t>(expected_length))
			throw HttpRequestError(invalid_body_length);
		body = body_content;
		body_exists = true;
	} else {
		if (method == "POST")
		{
			body = body_content;
			body_exists = true;
		}
		else
		{
			// Check if no body exist for other method (e.g, GET)
			if (!body_line.empty())
				throw HttpRequestError(invalid_body);
		}
	}
}