/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:09:04 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/09 16:44:16 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerBlock.hpp"

// assume IPv4
// assume only one port can exist on server at a time
/**
 * @brief	iter through the token till `;' special char is found
 * 			there a few different possible entry
 * 			only port number (assume any ip add)
 * 			[::]:7000 (special for any ip add) , bind to a port number
 * 			127.0.0.1:8080, a specfic ip add , bind to a port number
 * 			it will add the map, port as key and address info as value
 * 
 * @attention	assume our subject only need us to handle ipv4 (ipv6 is a pain lulz)
 * 				if need to add ipv6 then need to edit this sht lulz
 * 
 * @param	stream : the stringstream that from Parse class
 * 
 * @throws	ParsingError class , options: invalid_ip_add, repeated_port
*/
void	ServerBlock::processListen( std::stringstream &stream ) {
	std::string token;
	while (stream >> token) {
		if (token == ";")
			break ;
		sockaddr_t	add = {.sin_family = AF_INET}; // assume everything is ipv4
		std::size_t pos = token.find(':');
		if (pos == std::string::npos) {
			add.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else if (!token.compare(pos, 0, "[::]")) {
			add.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else {
			token[pos] = '\0';
			if (inet_pton(AF_INET, token.c_str(), &add.sin_addr) != 1)
				throw ParsingError(invalid_ip_add);
			token.erase(0, pos);
		}
		uint16_t	port = ft_stoi(token);
		add.sin_port = htons(port);
		if (listen.find(port) != listen.end())
			throw ParsingError(repeated_port);
		listen[port] = add;
		if (token[token.length() - 1] == ';')
			break ;
	}
}
