/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:32:08 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/17 03:18:59 by ychng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Parse.hpp"

// int	main(int ac, char **av, char **env) {
// 	(void)env;
// 	if (ac > 2)
// 		return (1); // too many arug error
// 	const char *ref;
// 	ref = "default.conf";
// 	if (ac == 2)
// 		ref = av[1];
// 	Server	server;
// 	Parse	parse(ref, server);
// 	// if (!parse.parseConfigFile())
// 	// 	return (parse.getError());
// 	parse.parseConfigFile();
// 	std::cout << "test\n" << server << '\n';
// 	return (0);
// 	// server.startServerLoop();
// 	return (0);
// }

#include "Http.hpp"

int main()
{
	try
	{
		std::string request = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
		Http http(request);

		http.print_request_line();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
