/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 01:46:51 by joshua            #+#    #+#             */
/*   Updated: 2024/09/18 01:20:11 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP
# include <set>
# include <algorithm>
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


	std::string content_type;
	std::string content_length;
	std::string host;
	std::string user_agent;
	std::string accept;
	std::string authorization;
	std::string connection;
	std::string accept_encoding;
	std::string accept_language;
	bool got_content_type;
	bool got_content_length;
	bool got_host;
	bool got_user_agent;
	bool got_accept;
	bool got_authorization;
	bool got_connection;
	bool got_accept_encoding;
	bool got_accept_language;
	bool got_request_headers;

	void parse_request_headers(const std::string& headers);
	void validate_headers();
	void trim(std::string& str);
	void to_lowercase(std::string& str);

	std::string body;
	bool got_body;
	void parse_request_body(const std::string& body);
public:
	Http(const std::string& msg);
	bool has_request_line() const;
	void print_request_line() const;

    bool has_content_type() const;
    bool has_content_length() const;
    bool has_host() const;
    bool has_user_agent() const;
    bool has_accept() const;
    bool has_authorization() const;
    bool has_connection() const;
    bool has_accept_encoding() const;
    bool has_accept_language() const;

    std::string get_content_type() const;
    std::string get_content_length() const;
    std::string get_host() const;
    std::string get_user_agent() const;
    std::string get_accept() const;
    std::string get_authorization() const;
    std::string get_connection() const;
    std::string get_accept_encoding() const;
    std::string get_accept_language() const;

	bool has_body() const;
	std::string get_body() const;
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

