/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:32:08 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/09 02:40:51 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

int	main(int ac, char **av, char **env) {
	if (ac > 2)
		return (1); // too many arug error
	const char *ref;
	ref = "default.conf";
	if (ac == 2)
		ref = av[1];
	Parse	parse(ref);
	if (!parse.parseConfigFile())
		return (parse.getError());
	
	return (0);
}