/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 23:19:09 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/10 21:11:21 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

const char *methods[] = {"GET", "POST", "PUT", "PATCH", "DELETE"};
const std::vector<std::string> Request::methodVector(methods, methods + sizeof(methods) / sizeof(char *));

std::string normalizePath(const std::string &path)
{
	std::vector<std::string> parts = utl::splitStringByDelim(path, '/');
	std::vector<std::string> stack;

	for (size_t i = 0; i < parts.size(); i++)
	{
		if (parts[i] == "." || parts[i].empty())
			continue; // Skip current directory references and empty tokens
		else if (parts[i] == "..")
		{
			if (!stack.empty())
				stack.pop_back(); // Pop the last directory aka move up in the directory tree
		}
		else
		{
			stack.push_back(parts[i]);
		}
	}
	std::stringstream result;
	for (size_t i = 0; i < stack.size(); i++)
		result << "/" << stack[i];
	return result.str().empty() ? "/" : result.str();
}

std::string urlDecode(const std::string &encoded)
{
	std::string result;
	result.reserve(encoded.length());
	for (size_t i = 0; i < encoded.length(); i++)
	{
		if (encoded[i] == '%' && i + 2 < encoded.length())
		{
			int value = 0;
			std::istringstream is(encoded.substr(i + 1, 2));
			if (is >> std::hex >> value)
			{
				result += static_cast<char>(value);
				i += 2;
			}
			else
				result += '%';
		}
		else if (encoded[i] == '+')
			result += ' ';
		else
			result += encoded[i];
	}
	return result;
}

std::string sanitizeUri(const std::string &uri)
{
	const std::string decodedPath = urlDecode(uri);
	const std::string normalizedPath = normalizePath(decodedPath);
	return (normalizedPath);
}

const std::string Request::getUri() const
{
	return this->_uri;
}

const std::string Request::getMethod() const
{
	return this->_method;
}

// Get the requested resource from the URI
// ie. HTTP/1.1 GET /path/to/resource -> /path/to/resource
const std::string Request::getRoute() const
{
	return this->_uri.substr(0, this->_uri.find('/', 1));
}

// Get the requested resource from the URI
// ie. /path/to/resource -> /to/resource
const std::string Request::getResource() const
{
	if (this->_uri.find('/', 1) == std::string::npos)
		return "";
	return this->_uri.substr(this->_uri.find('/', 1));
}

void Request::setUri(const std::string &uri)
{
	this->_uri = uri;
}

void Request::setMethod(const std::string &method)
{
	this->_method = method;
}

Request::Request(const std::string &requestString) : _logger(Logger("Request")), chunkSizeRemaining(0)
{
	if (requestString.empty())
		return;
	this->_logger.info("\n" + requestString);
	std::vector<std::string> split = utl::splitStringByDelim(requestString, '\n');
	std::vector<std::string> requestLine = utl::splitStringByDelim(split[0], ' ');
	const std::string &method = requestLine[0];
	this->setMethod(method);
	this->setUri(sanitizeUri(requestLine[1]));
	this->_version = "HTTP/1.1";

	split.erase(split.begin());
	this->parseHeaders(split);
}

Request::Request(Request const &src) : _logger(Logger("Request"))
{
	this->_method = src._method;
	this->_uri = src._uri;
	this->_version = src._version;
	this->_headers = src._headers;
	this->_body = src._body;
}

Request::~Request() {}

/**
 * read Buffer
 * if chunk size to read is 0,
 *  find \r\n to get the chunk size.
 */

int Request::processBody(const std::string &buffer)
{
	size_t needle;
	int len = 0;
	std::string tmp;

	size_t bufferIndex = 0;

    usleep(2 * 1000);

	if (this->getHeader("Transfer-Encoding").compare("chunked") == 0)
	{
		while (bufferIndex < buffer.size())
		{
			if (chunkSizeRemaining == 0)
			{
				needle = buffer.find("\r\n", bufferIndex);
				if (needle == std::string::npos)
				{
					this->_logger.error("Missing CRLF");
					return -1;
				}
				std::string sub = buffer.substr(bufferIndex, needle - bufferIndex);
				chunkSizeRemaining = strtol(sub.c_str(), NULL, 16);

				bufferIndex = needle + 2; // Skip past the \r\n

				if (chunkSizeRemaining == 0)
				{
					if (sub == "0")
					{
						this->_logger.info("Chunked last 0 found! Body len: " + utl::toString(this->_body.length()));
					}
					else
					{
						throw "error";
					}
					return 1;
				}
			}

			size_t chunkDataAvailable = std::min(chunkSizeRemaining, buffer.size() - bufferIndex);
			this->_body.append(buffer.substr(bufferIndex, chunkDataAvailable));

			std::cout << "\x1b[1A" << "\x1b[2K"; // Delete current line
			std::cout << "current lenght: " << this->_body.length() << std::endl;
			chunkSizeRemaining -= chunkDataAvailable;
			bufferIndex += chunkDataAvailable;

			if (chunkSizeRemaining == 0)
			{
				if (bufferIndex + 2 <= buffer.size() && buffer.substr(bufferIndex, 2) == "\r\n")
				{
					bufferIndex += 2; // Skip past the \r\n after the chunk
				}
				else
				{
					this->_logger.error("Invalid Chunked (Missing CRLF after chunk data)");
					return -1;
				}
			}
		}
	}
	else
	{
		tmp = this->getHeader("Content-Length");
		len = atoi(tmp.c_str());
		this->_body.append(buffer, 0, len - this->_body.length());
		if (this->_body.length() == (size_t)len)
			return 1;
	}
	return 0;
}

int Request::checkIfHandleWithoutBody(void)
{
	int len = 0;
	bool isChunk = false;

	if (this->getMethod() != "POST")
	{
		// if not POST method, only headers created when construct are
		// needed to handle the request;
		return 1;
	}

	len = atoi(this->getHeader("Content-Length").c_str());
	isChunk = this->getHeader("Transfer-Encoding") == "chunked";

	if ((len > 0 && !isChunk) || (len < 1 && isChunk))
	{
		this->_logger.info("has content length or chunked");
		return 0;
	}
	this->_logger.info("returning -1");

	return -1;
}

/************************ Request::NotAllowedException ************************/

Request::NotAllowedException::NotAllowedException(const std::string &reason) : _reason(reason) {}

Request::NotAllowedException::~NotAllowedException() throw() {}

/**
 * Reason why return 'const char*' instead of 'std::string'
 * - Exception safety: constructing std::string could potentially throw another
 * eception (eg bad_alloc_exception if system out of memory). Throwing an
 * exception while another exception is already being handled will lead to
 * std::terminate() being called.
 * - Historical reasons: The design of exceptions in C++ predates the STL
 * library and std::string.
 * - Compatability: The use of const char* ensures compatibility with C function
 * and libraries that might interact with C++ code.
 */
const char *Request::NotAllowedException::what() const throw()
{
	return _reason.c_str();
}
