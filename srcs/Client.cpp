/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/16 01:33:21 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) { }

Client::Client( const Client &src ) { (void)src; }

Client::~Client( void ) { }

Client&	Client::operator=( const Client &src ) { (void)src; return (*this); }

sockaddr_in_t&	Client::changeAddress( void ) { return(socket.changeAddress()); }

void	Client::addToReq( const std::string &add ) { request += add; }

void	Client::addToRes( const std::string &add ) { response += add; }

void	Client::finishRecv( void ) { finish_request = true; }

void	Client::finishSend( void ) { finish_write = true; }

bool	Client::checkReq( void ) const { return(finish_request); }

bool	Client::checkRes( void ) const { return(finish_write); }