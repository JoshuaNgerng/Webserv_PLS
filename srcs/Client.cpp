/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/23 15:18:47 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) { }

Client::Client( std::vector<ServerBlock>::iterator &it ) : server_ref(it) { }

Client::Client( const Client &src ) { (void)src; }

Client::~Client( void ) { }

Client&	Client::operator=( const Client &src ) { (void)src; return (*this); }

sockaddr_in_t&	Client::changeAddress( void ) { return(socket.changeAddress()); }

socklen_t&	Client::getSocklen( void ) { return(len); }

void	Client::setFd( int fd ) { socket_fd = fd; }

void	Client::addToReq( const std::string &add ) { request += add; }

void	Client::addToRes( const std::string &add ) { response += add; }

void	Client::addBytesSent( size_t add ) { bytes_sent += add; }

void	Client::finishRecv( void ) { finish_request = true; }

void	Client::finishSend( void ) { finish_write = true; }

bool	Client::checkReq( void ) const { return(finish_request); }

bool	Client::checkRes( void ) const { return(finish_write); }

std::vector<ServerBlock>::iterator	Client::getServerRef( void ) const { return(server_ref); }

int	Client::getFd( void ) const { return(socket_fd); }

const std::string&	Client::getRequest( void ) const { return(request); }

const std::string&	Client::getResponse( void ) const { return(response); }

size_t	Client::getBytesSent( void ) const { return(bytes_sent); }

std::ostream&	operator<<( std::ostream &o, const Client &ref ) {
	o << "Client socket fd: " << ref.getFd() << '\n';
	o << "Request status: " << ((ref.checkReq()) ? "complete" : "no ready") << '\n';
	o << "Request from Client\n" << ref.getRequest() << '\n';
	o << "Reponse status: " << ((ref.checkReq()) ? "complete" : "no ready") << '\n';
	o << "Reponse to Client\n" << ref.getResponse() << '\n';
	return (o);
}