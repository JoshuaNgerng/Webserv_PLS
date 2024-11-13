/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:32:08 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/13 10:31:32 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"
#include "CgiPipe.hpp"

static int start_server(const char *fname) {
	Server	server;
	{
		Parse	parse(fname, server);
		try {
			parse.parseConfigFile();
		}
		catch ( const std::exception &e ) {
			std::cerr << e.what() << '\n';
			return (1);
		}
	}
	std::cout << "done\n";
	// std::cout << "test server env\n";
	// for (size_t i = 0; Server::env[i]; i ++) {
		// std::cout << env[i] << '\n';
	// }
	std::signal(SIGINT, Server::signalHandler);
	server.startServerLoop();
	return (0);
}

int	main(int ac, char **av, char **env) {
	if (ac > 2)
		return (1); // too many arug error
	const char *ref;
	ref = "conf/default.conf";
	if (ac == 2)
		ref = av[1];
	Server::server_name = &(av[0][2]);
	Server::env = env;
	try {
		if (start_server(ref)) {
			return (1);
		}
	} catch ( CgiPipe::ChildProcess &child ) {
		std::cout << "\nchild process execve\n";
		return (child.execveCgiPipe());
	} catch ( const Server::SetupError &setup ) {
		return (1);
	} catch ( const std::exception &err ) {
		std::cerr << Server::server_name << ": [crit] " 
			<< err.what() << '\n';
	}
	return (0);
}
