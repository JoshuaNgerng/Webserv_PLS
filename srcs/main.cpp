/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:15:58 by jngerng           #+#    #+#             */
/*   Updated: 2024/06/13 17:15:21 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

int	handle_error(int type)
{
	string	errmsg(BOLDRED "Error: " RESET BOLDYELLOW);

	switch (type)
	{
	case 0:
		errmsg += "Programme required one arugment as config file";
		break;

	default:
		break;
	}
	errmsg += RESET "\n";
	std::cerr << errmsg;
	return (1);
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return (handle_error(0));
	
	return (0);
}