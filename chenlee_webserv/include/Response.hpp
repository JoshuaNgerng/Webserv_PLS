/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:42:28 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/10 14:39:51 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <sstream>
#include <fstream>

#include "HttpMessage.hpp"
#include "Logger.hpp"
#include "Utility.hpp"
// #include "ServerBlock.hpp"

#define BODY_SIZE 32768

class ServerBlock;

class Response : public HttpMessage
{
private:
	int _statusCode;
	std::string _responseString;
	Logger _logger;
	int _ready;
	bool _isHeaderSent;
	bool _isDone;
	ServerBlock* _serverBlock;
	std::string _projectDir;

public:
	Response();
	Response(ServerBlock &serverBlock);
	~Response();

	static IntStringMap statusMap;
	void setStatusCode(int status);
	int getStatusCode() const;
	bool getIsDone() const;
	std::string toString();
	void errorResponse(int status, const std::string &message);
	void truncateResponse(unsigned long length);
	int getReady(void);
	void setReady(int status);
	void setResponseString(std::string string);
};

#endif
