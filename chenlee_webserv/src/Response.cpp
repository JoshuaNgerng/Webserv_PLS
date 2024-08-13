/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 16:12:06 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/10 21:11:44 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "ServerBlock.hpp"

std::map<int, std::string> Response::statusMap;

Response::Response() : _logger(Logger("Response")), _ready(0)
{
	_statusCode = 200;
	statusMap[200] = "OK";
	statusMap[202] = "Accepted";
	statusMap[307] = "Temporary Redirect";
	statusMap[403] = "Forbidden";
	statusMap[404] = "Not Found";
	statusMap[405] = "Method Not Allowed";
	statusMap[413] = "Content Too Large";
	statusMap[500] = "Internal Server Error";
	_body = "";
	_isDone = false;
}

Response::Response(ServerBlock &serverBlock) : _logger(Logger("Response")), _ready(0), _serverBlock(&serverBlock)
{
	_statusCode = 200;
	statusMap[200] = "OK";
	statusMap[403] = "Forbidden";
	statusMap[404] = "Not Found";
	statusMap[405] = "Method not allowed";
	statusMap[500] = "Internal Server Error";
	_body = "";
	_isDone = false;
}

Response::~Response()
{
}

std::string Response::toString()
{
	if (this->_ready != 1)
		return "";
	std::stringstream responseStream;
	size_t bodyLen = this->_body.length();

	if (!_responseString.empty())
		return _responseString;

	if (!_isHeaderSent)
	{
		// Status line
		std::string statusMsg = (Response::statusMap.find(this->_statusCode))->second;
		responseStream << "HTTP/1.1 " << this->_statusCode << " " << statusMsg << "\r\n";

		if (bodyLen > 0)
		{
			if (bodyLen <= BODY_SIZE)
			{
				this->addHeader("Content-Length", utl::toString(this->getBody().length()));
			}
			else
			{
				this->addHeader("Transfer-Encoding", "chunked");
			}
		}
		// header
		for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
		{
			responseStream << it->first << ": " << it->second << "\r\n";
		}
		responseStream << "\r\n";
		_isHeaderSent = true;
	}

	bool isChunked = this->getHeader("Transfer-Encoding") == "chunked";
	// optional body
	if (bodyLen > 0)
	{
		if (isChunked)
		{
			size_t pos = 0; // Position in the body
			while (pos < bodyLen)
			{
				size_t remaining = bodyLen - pos;
				size_t chunkSize = remaining < BODY_SIZE ? remaining : BODY_SIZE; // Manually calculate the minimum
				responseStream << std::hex << chunkSize << "\r\n";
				responseStream << _body.substr(pos, chunkSize) << "\r\n";
				pos += chunkSize;
			}
			responseStream << "0\r\n\r\n"; // End of chunks
			_isDone = true;				   // Mark the response as complete
		}
		else
		{
			responseStream << _body;
			_isDone = true;
		}
	}
	else if (bodyLen == 0 && isChunked && !this->_isDone)
	{
		responseStream << "0\r\n\r\n";
		this->_isDone = true;
	}
	else if (bodyLen == 0 && !isChunked)
	{
		this->_isDone = true;
	}

	this->_responseString = responseStream.str();
	return this->_responseString;
}

std::string readFile(const std::string &filePath)
{
	std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);
	if (!fileStream)
	{
		std::cerr << "Cannot open file: " << filePath << std::endl;
		return "";
	}
	std::ostringstream content;
	content << fileStream.rdbuf();
	return content.str();
}

void Response::errorResponse(int statusCode, const std::string &errorMessage)
{
	std::string errorContent = "";

	if (Response::statusMap.count(this->_statusCode) == 0)
	{
		this->_logger.error("Invalid status code: " + utl::toString(this->_statusCode));
		statusCode = 500;
	}

	this->_logger.error(errorMessage);
	IntStringMap errorPages = this->_serverBlock->getErrorPages();
	if (errorPages.count(statusCode))
	{
		IntStringMap::iterator it = errorPages.find(statusCode);
		std::string filePath = this->_serverBlock->getProjectDir() + this->_serverBlock->getRoot() + "/" +  it->second;
		std::cout << "filePath: " << filePath << std::endl;
		errorContent = readFile(filePath);
	}
	if (errorContent.empty())
	{
		// If the error file is not found or is empty, use a default message
		errorContent = "<html><body><h1>" + std::to_string(statusCode) + " " + errorMessage + "</h1></body></html>";
	}

	setStatusCode(statusCode);
	setBody(errorContent);
	addHeader("Content-Type", "text/html");
	_isDone = true; // Indicate that the response is ready to be sent
}

void Response::truncateResponse(unsigned long length)
{
	if (this->_responseString.length() > length)
		this->_responseString = this->_responseString.substr(length);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void Response::setStatusCode(int status)
{
	this->_ready = 1;
	this->_statusCode = status;
}

int Response::getReady(void)
{
	return this->_ready;
}

void Response::setReady(int status)
{
	this->_ready = status;
}

int Response::getStatusCode() const
{
	return this->_statusCode;
}

bool Response::getIsDone() const
{
	return this->_isDone;
}
