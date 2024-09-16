/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpError.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 01:57:33 by ychng             #+#    #+#             */
/*   Updated: 2024/09/17 03:18:27 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPERROR_HPP
# define HTTPERROR_HPP
# include <string>

enum httperrorval
{
	incomplete_request_line = 0,
	missing_request_line,
	wrong_request_method,
	wrong_request_uri,
	wrong_request_http_version,
};

class HttpError : public std::exception
{
public:
	HttpError();
	HttpError(const HttpError& src);
	~HttpError();
	HttpError& operator=(const HttpError& src);
	virtual const char* what() const throw();

	HttpError(int val);
	void msgInit(int val);
private:
	std::string msg;
};

#endif
