/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:32:08 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/07 21:21:48 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"
#include "CgiPipe.hpp"

int	main(int ac, char **av, char **env) {
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
	Server::env = env;
	std::signal(SIGINT, Server::signalHandler);
	try {
		server.startServerLoop();
	} catch ( const CgiPipe::ChildProcess &error ) {
		return (error.getExitCode());
	}
	return (0);
}
