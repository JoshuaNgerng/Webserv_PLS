/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/16 17:12:32 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) { }

Client::Client( size_t server_index ) : server_ref(server_index) { }

Client::Client( const Client &src ) { (void)src; }

Client::~Client( void ) { }

Client&	Client::operator=( const Client &src ) { (void)src; return (*this); }

sockaddr_in_t&	Client::changeAddress( void ) { return(socket.changeAddress()); }

socklen_t&	Client::getSocklen( void ) { return(len); }

int&	Client::getFd( void ) { return(socket_fd); }

void	Client::addToReq( const std::string &add ) { request += add; }

void	Client::addToRes( const std::string &add ) { response += add; }

void	Client::addBytesSent( size_t add ) { bytes_sent += add; }

void	Client::finishRecv( void ) { finish_request = true; }

void	Client::finishSend( void ) { finish_write = true; }

bool	Client::checkReq( void ) const { return(finish_request); }

bool	Client::checkRes( void ) const { return(finish_write); }

int	Client::getServerRef( void ) const { return(server_ref); }

const std::string&	Client::getRequest( void ) const { return(request); }

const std::string&	Client::getResponse( void ) const { return(response); }

size_t	Client::getBytesSent( void ) const { return(bytes_sent); }
