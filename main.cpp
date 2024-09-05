/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:32:08 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/21 22:11:01 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

int	main(int ac, char **av, char **env) {
	(void)env;
	if (ac > 2)
		return (1); // too many arug error
	const char *ref;
	ref = "default.conf";
	if (ac == 2)
		ref = av[1];
	Server	server;
	Parse	parse(ref, server); 
	// if (!parse.parseConfigFile())
	// 	return (parse.getError());
	std::cout << "***Start of Parsing***\n";
	parse.parseConfigFile();
	std::cout << "***End of Parsing***\n\n";
	std::cout << "Start Test\n" << server << "End of Test\n";
	return (0);
	server.startServerLoop();
	return (0);
}
