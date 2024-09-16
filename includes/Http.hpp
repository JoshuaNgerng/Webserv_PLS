/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 01:46:51 by joshua            #+#    #+#             */
/*   Updated: 2024/09/17 03:02:39 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP
# include <set>
# include "Const.hpp"
# include "HttpError.hpp"

enum methods {
	GET,
	POST,
	DELETE
};

class Http
{
private:
	std::string method;
	std::string uri;
	std::string http_version;
	bool got_request_line;

	void parse_request_line(const std::string& request);
	void validate_method(const std::string& method) const;
	void validate_uri(const std::string& uri) const;
	void validate_http_version(const std::string& http_version) const;

public:
	Http(const std::string& msg);
	bool has_request_line() const;
	void print_request_line() const;
};

// class Http
// {
// private:
// 	std::string	request_header; // /r/n/r/n is end of header so after if got info its added into body
// 	std::string	request_body;
// 	std::string	reponse_header;
// 	std::string	reponse_body;
// 	bool		got_request_header;
// 	bool		got_request_body;
// 	bool		got_reponse_body;

// 	/* break down header */
// 	int			method; // GET, POST, DELETE
// 	std::string	path;

// 	std::string	host; // what this suppose to do
// 	std::vector<std::string>	user_agent;

// 	std::vector<std::string>	mutli_data;

// public:
// 	Http(/* args */);
// 	~Http();
// };

// Http::Http(/* args */)
// {
// }

// Http::~Http()
// {
// }


#endif

