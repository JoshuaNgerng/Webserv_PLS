/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpError.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 01:57:33 by ychng             #+#    #+#             */
/*   Updated: 2024/09/18 00:20:12 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPERROR_HPP
# define HTTPERROR_HPP
# include <string>

enum httperrorval
{
	incomplete_request_format = 0,
	incomplete_request_line,
	wrong_request_line_format,
	wrong_request_method,
	wrong_request_uri,
	wrong_request_http_version,
	incomplete_header_format,
	wrong_header_format,
	invalid_host,
	invalid_content_length,
	invalid_content_type,
	invalid_user_agent,
	invalid_accept,
	invalid_authorization,
	invalid_connection,
	invalid_accept_encoding,
	invalid_accept_language

};

class HttpError : public std::exception
{
public:
	HttpError();
	HttpError(const HttpError& src);
	virtual ~HttpError() throw();
	HttpError& operator=(const HttpError& src);
	virtual const char* what() const throw();

	HttpError(int val);
	void msgInit(int val);
private:
	std::string msg;
};

#endif
