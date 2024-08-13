/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:53:12 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/10 19:44:38 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMESSAGE_HPP
#define HTTPMESSAGE_HPP

#include <string>
#include <map>
#include <iostream>
#include <vector>

class HttpMessage
{
protected:
	std::string _version;
	std::map<std::string, std::string> _headers;
	std::string _body;

public:
	HttpMessage();
	~HttpMessage();

	void addHeader(const std::string &field, const std::string &value);
	void setBody(const std::string responseBody);
	const std::string getBody() const;
	const std::string getHeader(const std::string &field) const;
	const std::map<std::string, std::string> getAllHeaders() const;
	void parseHeaders(std::vector<std::string> string);
};

#endif
