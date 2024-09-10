/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 01:46:51 by joshua            #+#    #+#             */
/*   Updated: 2024/09/07 16:56:56 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP
# include "Const.hpp"

enum methods {
	GET,
	POST,
	DELETE
};

class Http
{
private:
	std::string	request_header; // /r/n/r/n is end of header so after if got info its added into body
	std::string	request_body;
	std::string	reponse_header;
	std::string	reponse_body;
	bool		got_request_header;
	bool		got_request_body;
	bool		got_reponse_body;

	/* break down header */
	int			method; // GET, POST, DELETE
	std::string	path;

	std::string	host; // what this suppose to do
	std::vector<std::string>	user_agent;

	std::vector<std::string>	mutli_data;

public:
	Http(/* args */);
	~Http();
};

Http::Http(/* args */)
{
}

Http::~Http()
{
}


#endif

