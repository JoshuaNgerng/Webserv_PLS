/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:32:08 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/24 19:39:26 by ychng            ###   ########.fr       */
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

// #include "Http.hpp"

// int main()
// {
// 	try
// 	{
// 		std::string request = "POST / HTTP/1.1\r\nContent-Type: application/json\r\nContent-Length: 15\r\n\r\n       {\"key\":\"value\"}";
//         Http http(request);

//         std::cout << "Content-Type: " << http.get_content_type() << std::endl;
//         std::cout << "Content-Length: " << http.get_content_length() << std::endl;
//         std::cout << "Body: " << http.get_body() << std::endl;
// 	}
// 	catch (const std::exception& e)
// 	{
// 		std::cout << e.what() << std::endl;
// 	}
// }

#include "HttpRequest.hpp"

int main()
{
	HttpRequest request;

	try
	{
		request.addRequestHeader("Content-Type: application/json");
		request.addRequestHeader("Host: example.com");
		request.addRequestHeader("user-Agent: MyHttpClient");
		std::cout << request.getContentType() << "\n";
		std::cout << request.getHost() << "\n";
		std::cout << request.getUserAgent() << "\n";
	}
	catch (const HttpRequestError& e)
	{
		std::cout << e.what() << std::endl;
	}
}
