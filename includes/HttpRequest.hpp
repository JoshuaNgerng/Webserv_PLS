/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:13:01 by ychng             #+#    #+#             */
/*   Updated: 2024/09/24 19:28:20 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "Const.hpp"
# include "HttpRequestError.hpp"
# include <iomanip>

class HttpRequest
{
public:
	HttpRequest();									// Constructor
	~HttpRequest();									// Destructor
	HttpRequest(const HttpRequest& src);			// Copy Constructor
	HttpRequest& operator=(const HttpRequest& src);	// Assignment Operator

	// Getters
	const std::string& getMethod() const;	
	const std::string& getUri() const;
	const std::string& getVersion() const;
	const std::string& getRequestLine() const;
	const std::string& getContentType() const;
	const std::string& getContentLength() const;
	const std::string& getHost() const;
	const std::string& getUserAgent() const;
	const std::string& getAccept() const;
	const std::string& getBody() const;

	// Setters
	//

	// Field existence checker
	enum Field { REQUEST_LINE, HEADER, BODY, CONTENT_TYPE, CONTENT_LENGTH, HOST, USER_AGENT, ACCEPT }; 	// Enum to specify what to check
	bool hasField(Field field) const;			// Check if specify field exists

	// Parsing methods
	bool parseRequestLine(const std::string& request_line);
	bool parseHeaders(const std::string& header_lines);
	bool parseBody(const std::string& body_line);

	// Methods to add headers and body
	void addRequestHeader(const std::string& header_line);
	void addRequestBody(const std::string& body_line);

private:
	// Request Line
	std::string method;					// HTTP method (GET, POST, DELETE)
	std::string uri;					// Uniform Resource Identifier (request target) 
	std::string version;				// HTTP version (e.g., "HTTP/1.1")
	std::string request_line;
	
	// Headers
	std::string content_type;
	std::string content_length;
	std::string host;
	std::string user_agent;
	std::string accept;

	// Body
	std::string body;
	
	// Existence flags
	bool request_line_exists;
	bool header_exists;
	bool body_exists;
	bool content_type_exist;
	bool content_length_exist;
	bool host_exists;
	bool user_agent_exists;
	bool accept_exists;

	// Method to validate the request line
	bool isValidMethod(const std::string& method) const;
	bool isValidUri(const std::string& uri) const;
	bool isValidVersion(const std::string& version) const;

	// Method to validate the headers line
	bool isValidHeaders() const;

	// Helper functions for parsing
	void trimSpacesAround(std::string& str) const; // Trim whitespace around a string
	void toLowercase(std::string& str) const; // Convert a string to lowercase
};

#endif
