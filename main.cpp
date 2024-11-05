/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:32:08 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/05 17:05:12 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

int	main(int ac, char **av, char **env) {
	(void)env;
	if (ac > 2)
		return (1); // too many arug error
	const char *ref;
	ref = "conf/default.conf";
	if (ac == 2)
		ref = av[1];
	Server	server;
	{
		Parse	parse(ref, server);
		try {
			parse.parseConfigFile();
		}
		catch ( const std::exception &e ) {
			std::cerr << e.what() << '\n';
			return (1);
		}
	}
	std::cout << "done\n";
	Server::server_name = &av[0][2];
	std::signal(SIGINT, Server::signalHandler);
	server.startServerLoop();
	return (0);
}
