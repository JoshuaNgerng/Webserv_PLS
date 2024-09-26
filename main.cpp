/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychng <ychng@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:32:08 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/26 18:18:20 by ychng            ###   ########.fr       */
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

// #include "HttpRequest.hpp"

// int main()
// {
// 	HttpRequest request;

// 	try
// 	{
// 		request.addRequestHeader("Content-Type: application/json");
// 		request.addRequestHeader("Host: example.com");
// 		request.addRequestHeader("user-Agent: MyHttpClient");
// 		std::cout << request.getContentType() << "\n";
// 		std::cout << request.getHost() << "\n";
// 		std::cout << request.getUserAgent() << "\n";
// 	}
// 	catch (const HttpRequestError& e)
// 	{
// 		std::cout << e.what() << std::endl;
// 	}
// }














#include <iostream>

/**
 ** @brief Adds spaces around '{', '}', and ';', if needed.
 ** @return A formatted string with spaces inserted where needed.
 */
std::string insertWhiteSpace(const std::string& input)
{
	std::string result;
	bool last_char_was_space = false;

	for (size_t i = 0; i < input.length(); i++)
	{
		char current = input[i];

		// Check if the character is '{', '}', or ';'
		if (current == '{' || current == '}' || current == ';')
		{
			if (i > 0 && !std::isspace(input[i - 1]) && !last_char_was_space)
				result += ' ';
			result += current;
			if (i + 1 < input.length() && !std::isspace(input[i + 1]))
			{
				result += ' ';
				last_char_was_space = true;
			}
			else
				last_char_was_space = false;
		}
		else
		{	
			result += current;
			last_char_was_space = std::isspace(current);
		}
	}
	return result;
}

int main()
{
	std::string input = "server{\r\nlisten 80;server_name lulz;}\r\n";
	std::string result = insertWhiteSpace(input);
	std::cout << result;
}
