/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 17:44:12 by jngerng           #+#    #+#             */
/*   Updated: 2024/06/13 17:42:18 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerSocket.hpp>

int	main(int ac, char **av)
{
	(void)av;
	if (ac > 2)
		return (1);
	ServerSocket	test;
	try
	{
		test.establishConnection();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	string	msg = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	string	reponse = "<html><h1>Hello world</h1></html?";
	msg.append(std::to_string(reponse.size()));
	msg.append("\n\n");
	msg.append(reponse);
	for (int i = 0; i < 7; i ++)
	{
		std::cout << "Test new connect\n";
		test.acceptNewConnection();
		test.setPollTimeOut(100);
		try
		{
			std::cout << "Test receive connect\n";
			// test.recvFromNewConnection(0);
			std::cout << "Received:\t" << test.recvFromNewConnection() << '\n';
			test.sendToNewConnection(msg);
			std::cout << "test client api " << test.IPAddressRep(test.getClientAdd()) << '\n';
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	std::cout << "Attempt to connect 2 times\n";
	return (0);
}
