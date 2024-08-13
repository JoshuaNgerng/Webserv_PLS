/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:58:26 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/10 21:12:40 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticFileHandler.hpp"

std::string getMimeType(const std::string &filePath)
{
	size_t dotPos = filePath.rfind('.');
	if (dotPos != std::string::npos)
	{
		std::string ext = filePath.substr(dotPos);
		if (ext == ".html")
			return "text/html";
		else if (ext == ".css")
			return "text/css";
		else if (ext == ".js")
			return "application/javascript";
		else if (ext == ".png")
			return "image/png";
		else if (ext == ".jpg" || ext == ".jpeg")
			return "image/jpeg";
		else if (ext == ".gif")
			return "image/gif";
	}
	return "application/octet-stream"; // Default MIME type
}

bool serveFile(const std::string &filePath, Response &response)
{
	std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
	if (!file)
	{
		response.errorResponse(404, "File not found");
		return true;
	}

	// Use a stringstream to read the file into a string
	std::stringstream buffer;
	buffer << file.rdbuf();
	// Convert the stringstream into a string
	std::string content = buffer.str();

	file.close();

	response.setStatusCode(200);
	response.setBody(content);
	response.addHeader("Content-Type", getMimeType(filePath));
	return true;
}

bool handleGetRequest(const Request &request, Response &response, const RouteDetails &routeDetail, const std::string &fullPath, const struct stat &path_stat)
{
	(void)request;
	if (S_ISDIR(path_stat.st_mode) && !routeDetail.index.empty())
	{
		return serveFile(fullPath + "/" + routeDetail.index, response);
	}
	else if (S_ISREG(path_stat.st_mode))
	{
		return serveFile(fullPath, response);
	}
	else
		return false;
}

std::string getCurrentTime()
{
	time_t rawtime;
	time(&rawtime);
	struct tm *timeinfo = gmtime(&rawtime);
	char buffer[40];
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
	return buffer;
}

static std::string find_filename(const std::string &contentDisposition)
{
	std::string cd = contentDisposition + ";";
	std::map<std::string, std::string> attributes;
	size_t start = 0;
	size_t end = 0;

	while ((end = cd.find(";", start)) != std::string::npos)
	{
		std::string attribute = cd.substr(start, end - start);
		size_t equalsPos = attribute.find("=");
		if (equalsPos != std::string::npos)
		{
			std::string key = attribute.substr(0, equalsPos);
			std::string value = attribute.substr(equalsPos + 1);

			// Remove potential quotes and semicolons around the value
			size_t firstQuote = value.find("\"");
			size_t lastQuote = value.rfind("\"");
			if (firstQuote != std::string::npos && lastQuote != std::string::npos && firstQuote != lastQuote)
			{
				value = value.substr(firstQuote + 1, lastQuote - firstQuote - 1);
			}

			attributes[key] = value;
		}
		start = end + 2; // skip "; "
	}

	if (attributes.find("filename") != attributes.end())
		return attributes["filename"];

	return "newfile";
}

std::string extractHeader(const std::string &headerName, const std::string &headers)
{
	std::istringstream stream(headers);
	std::string line;
	while (std::getline(stream, line))
	{
		size_t colonPos = line.find(':');
		if (colonPos != std::string::npos)
		{
			std::string name = line.substr(0, colonPos);
			if (name == headerName)
			{
				// Remove spaces after the colon and before the value
				size_t valueStart = line.find_first_not_of(" ", colonPos + 1);
				if (valueStart != std::string::npos)
				{
					return line.substr(valueStart);
				}
			}
		}
	}
	return "";
}

std::string process_file(const Request &request, std::string *file_name)
{
	std::string result;

	std::string contentType = request.getHeader("Content-Type");
	if (contentType.empty() || contentType.find("multipart/form-data;") != 0)
	{
		return result;
	}

	std::string boundary = "--" + contentType.substr(contentType.find("boundary=") + 9);
	std::string body = request.getBody();
	std::vector<std::string> parts;
	size_t pos = 0, old_pos = 0;

	// Extract parts based on the boundary
	while ((pos = body.find(boundary, pos)) != std::string::npos)
	{
		if (pos != old_pos)
		{
			std::string part = body.substr(old_pos, pos - old_pos);
			parts.push_back(part);
		}
		pos += boundary.length() + 2; // Move past the boundary and CRLF
		old_pos = pos;
	}

	// Process each part to find files
	for (size_t i = 0; i < parts.size(); ++i)
	{
		std::string part = parts[i];
		size_t headerEnd = part.find("\r\n\r\n");
		if (headerEnd == std::string::npos)
			continue;

		std::string headers = part.substr(0, headerEnd);
		std::string body = part.substr(headerEnd + 4);
		std::string disposition = extractHeader("Content-Disposition", headers);
		if (!disposition.empty())
		{
			*file_name = find_filename(disposition);
			result += body;
		}
	}

	return result;
}

bool handlePostRequest(const Request &request, Response &response, const RouteDetails &RouteDetails, const std::string &fullPath)
{
	(void)RouteDetails;

	struct stat path_stat;
	stat(fullPath.c_str(), &path_stat);
	std::string finalPath;
	std::string body;

	if (S_ISDIR(path_stat.st_mode))
	{
		std::string fileName;
		body = process_file(request, &fileName);
		finalPath = fullPath + "/" + fileName;
	}
	else
	{
		finalPath = fullPath;
		body = request.getBody();
	}
	std::ofstream file(finalPath.c_str(), std::ios::out | std::ios::binary);
	if (!file)
		response.errorResponse(500, "Failed to open file.");
	else
	{
		file.write(body.data(), body.size());
		if (file.fail())
			response.errorResponse(500, "Failed to save file.");
		else
		{
			std::string resBody = "File uploaded successfully.";
			response.setStatusCode(200);
			response.setBody(resBody);
			response.addHeader("Date", getCurrentTime());
			response.addHeader("Content-Type", "text/html");
			response.addHeader("Content-Length", utl::toString(resBody.length() + 1));
		}
		file.close();
	}
	return true;
}

bool handleDeleteRequest(Response &response, const std::string &fullPath, const struct stat &path_stat)
{
	if (S_ISREG(path_stat.st_mode))
	{
		if (remove(fullPath.c_str()) == 0)
			response.setStatusCode(202);
		else
			response.errorResponse(500, "Unable to delete file");
		return true;
	}
	else
	{
		response.errorResponse(403, "Cannot delete directory");
		return true;
	}
	return false;
}

StaticFileHandler::StaticFileHandler() {}

StaticFileHandler::~StaticFileHandler() {}

bool StaticFileHandler::checkIfHandle(const Request &request, RouteDetails &routeDetail, const std::string &fullPath)
{
	(void)request;
	(void)routeDetail;
	(void)fullPath;
	return true;
}

bool StaticFileHandler::handleRequest(const Request &request, Response &response, RouteDetails &routeDetail, const std::string &fullPath)
{
	// Early handling for POST requests (for file uploads)
	if (request.getMethod() == "POST" && (routeDetail.allowedMethods & POST))
		return handlePostRequest(request, response, routeDetail, fullPath);

	// Use stat to check file or directory status for GET/DELETE methods
	struct stat path_stat;
	if (stat(fullPath.c_str(), &path_stat) != 0)
	{
		response.errorResponse(404, "Resource not found");
		return true;
	}
	if (request.getMethod() == "GET" && (routeDetail.allowedMethods & GET))
		return handleGetRequest(request, response, routeDetail, fullPath, path_stat);
	else if (request.getMethod() == "DELETE" && (routeDetail.allowedMethods & DELETE))
		return handleDeleteRequest(response, fullPath, path_stat);
	else
		return false;
}
