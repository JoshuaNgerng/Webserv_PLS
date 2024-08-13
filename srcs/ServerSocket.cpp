/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 00:56:36 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/13 13:08:34 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"

ServerSocket::ServerSocket( void ) :
	Tcp(), backlog(10), socket_fd_new(0), timeout(1000),
	pollfd((struct pollfd){.fd = 0, .events = POLLIN, .revents = 0}) { }

ServerSocket::ServerSocket( int backlog_ ) :
	Tcp(), backlog(backlog_), socket_fd_new(0), timeout(1000),
	pollfd((struct pollfd){.fd = 0, .events = POLLIN, .revents = 0}) { }

ServerSocket::ServerSocket( string ip_addr_, int port ) :
	Tcp(ip_addr_, port), backlog(10), socket_fd_new(0), timeout(1000),
	pollfd((struct pollfd){.fd = 0, .events = POLLIN, .revents = 0}) { }

ServerSocket::ServerSocket( string ip_addr_, int port, int backlog_ ) :
	Tcp(ip_addr_, port), backlog(backlog_), socket_fd_new(0), timeout(1000),
	pollfd((struct pollfd){.fd = 0, .events = POLLIN, .revents = 0}) { }

ServerSocket::ServerSocket( string ip_addr_, int port,
	int domain, int service, int protocol ) : 
	Tcp(ip_addr_, port, domain, service, protocol ),
	backlog(10), socket_fd_new(0), timeout(1000),
	pollfd((struct pollfd){.fd = 0, .events = POLLIN, .revents = 0} ) { }

ServerSocket::ServerSocket( string ip_addr_, int port, int backlog_,
	int domain, int service, int protocol ) : 
	Tcp(ip_addr_, port, domain, service, protocol),
	backlog(backlog_), socket_fd_new(0), timeout(1000),
	pollfd((struct pollfd){.fd = 0, .events = POLLIN, .revents = 0}) { }


ServerSocket::~ServerSocket( void )
{ if (socket_fd_new > 0) {close(socket_fd_new);} }

void	ServerSocket::establishConnection( void )
{
	startServer();
	if (bind(socket_fd, (sockaddr *)&address, sizeof(address)) < 0)
		throw TcpError("Cannot bind socket");
	if (listen(socket_fd, backlog) < 0)
		throw TcpError("Cannot listening to socket");
	std::cout << "Listening to " << ip_addr << ":" << m_port << '\n';
}

void	ServerSocket::establishConnection( int backlog_ )
{
	backlog = backlog_;
	startServer();
	if (bind(socket_fd, (sockaddr *)&address, sizeof(address)) < 0)
		throw TcpError("Cannot bind socket");
	if (listen(socket_fd, backlog) < 0)
		throw TcpError("Cannot listening to socket");
}

void	ServerSocket::acceptNewConnection( void )
{
	socklen_t	len = sizeof(address_new);
	if (socket_fd_new > 0)
		close(socket_fd_new);
	if ((socket_fd_new = accept(socket_fd, (struct sockaddr *)&address_new, &len)) < 0)
		throw TcpError("Unable to accept new connection");
	pollfd.fd = socket_fd_new;
}

void	ServerSocket::setPollTimeOut( int timeout_ ) { timeout = timeout_; }

void	ServerSocket::setPollTimeOut( short events, short revents, int timeout_ )
{
	pollfd.events = events;
	pollfd.revents = revents;
	timeout = timeout_;
}

string	ServerSocket::readFromNewConnection( void )
{
	string	out;

	for (ssize_t no_bytes_received = read(socket_fd_new, buffer, capacity);
		no_bytes_received;
		no_bytes_received = read(socket_fd_new, buffer, capacity))
	{
		if (no_bytes_received < 0)
			throw TcpError("Read from client failed");
		buffer[no_bytes_received] = '\0';
		out += buffer;
	}
	return (out);
}

string	ServerSocket::recvFromNewConnection( int flag )
{
	int		check;
	string	out;
	ssize_t no_bytes_received;

	for (check = poll(&pollfd, 1, timeout); check > 0;
		check = poll(&pollfd, 1, timeout))
	{
		if (pollfd.revents & (POLLERR | POLLHUP))
			break ;
		if(pollfd.revents & POLLIN)
		{
			no_bytes_received = recv(pollfd.fd, buffer, capacity, flag);
			if (no_bytes_received == 0)
				break ;
			if (no_bytes_received < 0)
				throw TcpError("Failed to recv from client");
			buffer[no_bytes_received] = '\0';
			std::cout << "testing " << no_bytes_received << '\n';
			out += buffer;
			std::cout << "str\n" << buffer;
		}
	}
	// for (ssize_t no_bytes_received = recv(socket_fd_new, buffer, capacity, flag);
	// 	no_bytes_received > 0;
	// 	no_bytes_received = recv(socket_fd_new, buffer, capacity, flag))
	// {
	// 	if (no_bytes_received < 0)
	// 		throw TcpError("Read from client failed");
	// 	buffer[no_bytes_received] = '\0';
	// 	std::cout << "testing " << no_bytes_received << '\n';
	// 	out += buffer;
	// }
	// std::cout << "testing " << recv(socket_fd_new, buffer, capacity, flag) << '\n';
	// std::cout << "testing " << recv(socket_fd_new, buffer, capacity, flag) << '\n';
	return (out);
}

string	ServerSocket::recvFromNewConnection( void ) { return (recvFromNewConnection(0)); }

int	ServerSocket::writeToNewConnection( string message )
{
	size_t		bytes_send;
	ssize_t		check;
	const char	*ptr = message.c_str();

	for (bytes_send = 0, check = write(socket_fd_new, ptr, message.length());
		 bytes_send < message.length();
		 check = write(socket_fd_new, ptr + bytes_send, message.length() - bytes_send),
		 bytes_send += (size_t)check)
	{
		if (check < 0)
			throw TcpError("Write to client failed");
		if (check == 0)
			throw TcpError("Client connection have been closed");
	}
	return (0);
}

int	ServerSocket::sendToNewConnection( string message, int flag )
{
	size_t		bytes_send;
	ssize_t		check;
	const char	*ptr = message.c_str();

	for (bytes_send = 0, check = send(socket_fd_new, ptr, message.length(), flag);
		 bytes_send < message.length();
		 check = send(socket_fd_new, ptr + bytes_send, message.length() - bytes_send, flag),
		 bytes_send += (size_t)check)
	{
		if (check < 0)
			throw TcpError("Write to client failed");
		if (check == 0)
			throw TcpError("Client connection have been closed");
	}
	return (0);
}

int	ServerSocket::sendToNewConnection( string message ) { return (sendToNewConnection(message, 0)); }

int	ServerSocket::getBackLog( void ) const { return (backlog); }

sa	ServerSocket::getClientAdd( void ) const { return (address_new); }
