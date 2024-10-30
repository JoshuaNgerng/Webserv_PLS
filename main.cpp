/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:32:08 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/30 15:40:48 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

Server	*server_ptr = NULL;

void cleanup( int signum ) {
	// cout << "Interrupt signal (" << signum << ") received.\n";
	if (signum != SIGINT) {
		return ;
	}
	if (server_ptr) {
		swap_clear(*server_ptr);
	}
	exit(0);  
}

int	main(int ac, char **av, char **env) {
	(void)env;
	if (ac > 2)
		return (1); // too many arug error
	const char *ref;
	ref = "conf/default.conf";
	if (ac == 2)
		ref = av[1];
	Server	server;
	Parse	parse(ref, server);
	parse.parseConfigFile();
	server_ptr = &server;
	std::signal(SIGINT, cleanup);
	server.startServerLoop();
	return (0);
}
