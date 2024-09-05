/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:21:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/05 15:40:09 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) : server_ref(), socket_ref(), reponse_ref(),
	socket_fd(-1), reponse_fd(-1), socket(), len(0), request(), response(),
	bytes_sent(0), is_cgi(false), data_ready(false),
	is_data_avaliable(true), finish_request(false),
	finish_response(false), attempts(0) { }

Client::Client( std::vector<ServerBlock>::iterator &it ) :
	server_ref(it), socket_ref(), reponse_ref(),
	socket_fd(-1), reponse_fd(-1), socket(), len(0), request(), response(),
	bytes_sent(0), is_cgi(false), data_ready(false),
	is_data_avaliable(true), finish_request(false),
	finish_response(false), attempts(0) { }

Client::Client( const Client &src ) : // help me fill in with src T_T or else have bug
	server_ref(src.server_ref), socket_ref(), reponse_ref(),
	socket_fd(-1), reponse_fd(-1), socket(), len(0), request(), response(),
	bytes_sent(0), is_cgi(false), data_ready(false),
	is_data_avaliable(true), finish_request(false),
	finish_response(false), attempts(0) { }

Client::~Client( void ) { }

Client&	Client::operator=( const Client &src ) {
	if (this == &src)
		return (*this);
	server_ref = src.server_ref; socket_fd = src.socket_fd;
	socket = src.socket; len = src.len; request = src.request;
	response = src.response; bytes_sent = src.bytes_sent;
	finish_request = src.finish_request; finish_response = src.finish_response;
	return (*this);
}

//placeholder for now
int	Client::fetchDataFd( void ) { return (-1); }

void	Client::resetDataFd( void ) { }

sockaddr_in_t&	Client::changeAddress( void ) { return(socket.changeAddress()); }

socklen_t&	Client::getSocklen( void ) { return(len); }

void	Client::setSocketFd( int fd ) { socket_fd = fd; }

void	Client::setReponseFd( int fd ) { reponse_fd = fd; }

int	Client::getResource( void ) { return (-1); } // place holder

void	Client::addToRequest( const std::string &add ) { request += add; }

void	Client::addToResponse( const std::string &add ) { response += add; }

void	Client::addToRequest( const char *str, size_t len = std::string::npos )
{
	if (len == std::string::npos)
	{
		request.append(str);
		return ;
	}
	request.append(str, len);
}

void	Client::addToResponse( const char *str, size_t len = std::string::npos )
{
	if (len == std::string::npos)
	{
		response.append(str);
		return ;
	}
	response.append(str, len);
}

void	Client::addBytesSent( ssize_t add ) { bytes_sent += static_cast<size_t>(add); }

void	Client::finishReceiveRequest( void ) { finish_request = true; }

void	Client::finishSendReponse( void ) { finish_response = true; }

void	Client::dataIsReady( void ) { data_ready = true; }

bool	Client::checkRequest( void ) const { return(finish_request); }

bool	Client::checkResponse( void ) const { return(finish_response); }

bool	Client::isDataReady( void ) const { return(data_ready); }

bool	Client::isDataAvaliable( void ) const { return(is_data_avaliable); }

bool	Client::isTimeout( void ) const { return (attempts >= max_attempt); }

std::vector<ServerBlock>::iterator	Client::getServerRef( void ) const { return(server_ref); }

int	Client::getSocketFd( void ) const { return(socket_fd); }

int	Client::getReponseFd( void ) const { return(reponse_fd); }

const std::string&	Client::getRequest( void ) const { return(request); }

const std::string&	Client::getResponse( void ) const { return(response); }

size_t	Client::getBytesSent( void ) const { return(bytes_sent); }

std::ostream&	operator<<( std::ostream &o, const Client &ref ) {
	o << "Client socket fd: " << ref.getSocketFd() << '\n';
	o << "Client reponse fd: " << ref.getReponseFd() <<
		", reponse resource status: " << ((ref.isDataReady()) ? "ready" : "not ready") << '\n';
	o << "Request status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	o << "Request from Client\n" << ref.getRequest() << '\n';
	o << "Reponse status: " << ((ref.checkRequest()) ? "complete" : "not ready") << '\n';
	o << "Reponse to Client\n" << ref.getResponse() << '\n';
	return (o);
}
