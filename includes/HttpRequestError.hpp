/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestError.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 19:06:32 by ychng             #+#    #+#             */
/*   Updated: 2024/09/24 19:14:23 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTPPREQUESTERROR_HPP
# define HTPPREQUESTERROR_HPP

# include <string>

enum HttpRequestErrorCode
{
    missing_request_line,
    trailing_spaces,
    incomplete_request_line,
    invalid_method,
    invalid_uri,
    invalid_version,
    invalid_content_type,
    invalid_content_length,
    invalid_host,
    invalid_user_agent,
    invalid_accept,
    incomplete_header,
    invalid_body_length,
    invalid_body,
};

class HttpRequestError : public std::exception
{
public:
	HttpRequestError();
	virtual	~HttpRequestError();
	HttpRequestError(const HttpRequestError& src);
	HttpRequestError& operator=(const HttpRequestError& src);

	HttpRequestError(int val);
	virtual const char* what() const throw();
private:
	void msgInit(int val);
	std::string msg;
};

#endif