/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:38:51 by joshua            #+#    #+#             */
/*   Updated: 2024/09/19 19:34:16 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Const.hpp"

/*

<html>
<head><title>413 Request Entity Too Large</title></head>
<body>
<center><h1>413 Request Entity Too Large</h1></center>
<hr><center>nginx/1.18.0 (Ubuntu)</center>
</body>
</html>
*/

enum error_status {
	s300,
	s301,
	s302,
	s303,
	s305,
	s306,
	s307,
	s308,
	s400,
	s401,
	s403,
	s404,
	s405,
	s501
};

class ErrorPage
{
	public:
		ErrorPage( void );
		~ErrorPage( void );

		void		addErrorPage( int status_code, const std::string &path );
		bool		searchErrorPage( std::string &path, int status_code ) const;
		std::string	makeDefaultMsg( int status_code,
									const char *status_msg,
									const char *distro ) const;
		void		clear( void );
	private:
		static const char 			*default_template;
		std::map<int, std::string>	errmsg_config;

};
