/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/04 14:36:46 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) : server_ref(), socket_fd(-1), reponse_fd(-1), socket(),
	len(0), request(), response(), bytes_sent(0), get_reponse(false),
	finish_request(false), finish_write(false), status_code(0) { }

Client::Client( std::vector<ServerBlock>::iterator &it ) :
	server_ref(it), socket_fd(-1), reponse_fd(-1), socket(), len(0), request(),
	response(), bytes_sent(0), get_reponse(false), finish_request(false),
	finish_write(false), status_code(0) { }

Client::Client( const Client &src ) :
	server_ref(src.server_ref), socket_fd(src.socket_fd), reponse_fd(-1),
	socket(src.socket), len(src.len), request(src.request),
	response(src.response), bytes_sent(src.bytes_sent),
	get_reponse(src.get_reponse), finish_request(src.finish_request),
	finish_write(src.finish_write), status_code(src.status_code) { }

Client::~Client( void ) { }

Client&	Client::operator=( const Client &src ) {
	if (this == &src)
		return (*this);
	server_ref = src.server_ref; socket_fd = src.socket_fd;
	socket = src.socket; len = src.len; request = src.request;
	response = src.response; bytes_sent = src.bytes_sent;
	finish_request = src.finish_request; finish_write = src.finish_write;
	status_code = src.status_code;
	return (*this);
}

sockaddr_in_t&	Client::changeAddress( void ) { return(socket.changeAddress()); }

socklen_t&	Client::getSocklen( void ) { return(len); }

void	Client::setSocketFd( int fd ) { socket_fd = fd; }

void	Client::setReponseFd( int fd ) { reponse_fd = fd; }

int	Client::getResource( void ) { return (-1); } // place holder

void	Client::addToReq( const std::string &add ) { request += add; }

void	Client::addToRes( const std::string &add ) { response += add; }

void	Client::addToReq( const char *str, size_t len = std::string::npos )
{
	if (len == std::string::npos)
	{
		request.append(str);
		return ;
	}
	request.append(str, len);
}

void	Client::addBytesSent( size_t add ) { bytes_sent += add; }

void	Client::finishRecv( void ) { finish_request = true; }

void	Client::finishSend( void ) { finish_write = true; }

bool	Client::checkRequest( void ) const { return(finish_request); }

bool	Client::checkResponse( void ) const { return(finish_write); }

bool	Client::isReponseReady( void ) const { return(get_reponse); }

std::vector<ServerBlock>::iterator	Client::getServerRef( void ) const { return(server_ref); }

int	Client::getSocketFd( void ) const { return(socket_fd); }

int	Client::getReponseFd( void ) const { return(reponse_fd); }

const std::string&	Client::getRequest( void ) const { return(request); }

const std::string&	Client::getResponse( void ) const { return(response); }

size_t	Client::getBytesSent( void ) const { return(bytes_sent); }

std::ostream&	operator<<( std::ostream &o, const Client &ref ) {
	o << "Client socket fd: " << ref.getSocketFd() << '\n';
	o << "Client reponse fd: " << ref.getReponseFd() <<
		", reponse resource status: " << ((ref.isReponseReady()) ? "ready" : "not ready") << '\n';
	o << "Request status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	o << "Request from Client\n" << ref.getRequest() << '\n';
	o << "Reponse status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	o << "Reponse to Client\n" << ref.getResponse() << '\n';
	return (o);
}
