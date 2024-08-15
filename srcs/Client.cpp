/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/15 09:58:26 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) { }

Client::Client( const Client &src ) { (void)src; }

Client::~Client( void ) { }

Client&	Client::operator=( const Client &src ) { (void)src; return (*this); }

sockaddr_in_t&	Client::changeAddress( void ) { return(socket.changeAddress()); }
