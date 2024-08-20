/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:09:04 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/21 01:00:19 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerBlock.hpp"

ServerBlock::ServerBlock( void ) { }

ServerBlock::ServerBlock( const ServerBlock &src ) { (void)src; }

ServerBlock::~ServerBlock( void ) { }

ServerBlock&	ServerBlock::operator=( const ServerBlock &src ) { (void)src; return (*this); }

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
 * 				assume https dont exist for now
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
		Socket	socket(AF_INET);// assume everything is ipv4
		std::size_t pos = token.find(':');
		if (pos == std::string::npos) {
			socket.changeAddress().sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else if (!token.compare(pos, 0, "[::]")) {
			socket.changeAddress().sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else {
			token[pos] = '\0';
			if (inet_pton(AF_INET, token.c_str(), &socket.changeAddress().sin_addr) != 1)
				throw ParsingError(invalid_ip_add);
			token.erase(0, pos);
		}
		uint16_t	port = std::atoi(token.c_str());//ft_stoi(token);
		socket.changeAddress().sin_port = htons(port);
		if (checkDupSocket(socket))
			throw ParsingError(repeated_port);
		listen.push_back(socket);
		if (token[token.length() - 1] == ';')
			break ;
	}
	// std::cout << "process end token: " << token << '\n';
}

void	ServerBlock::processSingleToken( std::string &dst, std::stringstream &stream ) {
	std::string	token;
	bool		check = false;
	while (stream >> token)
	{
		if (token == ";")
			break ;
		if (check)
			break ; // `;' not detected?
		if (token[token.length() - 1] == ';') {
			dst = token.substr(0, token.length() - 1);
			break ;
		}
		dst = token;
		check = true;
	}
	if (!check)
		return ; // empty slot??
	// std::cout << "process end token: " << token << '\n';
}

// assume theres only one servername for now
void	ServerBlock::processServerName( std::stringstream &stream ) {
	std::string	token;
	while (stream >> token)
	{
		if (token == ";")
			break ;
		if (token[token.length() - 1] == ';') {
			server_name = token.substr(0, token.length() - 1);
			break ;
		}
		server_name = token;
	}
}

// assume only one root
void	ServerBlock::processRoot( std::stringstream &stream ) {
	std::string	token;
	while (stream >> token)
	{
		if (token == ";")
			break ;
		if (token[token.length() - 1] == ';') {
			root = token.substr(0, token.length() - 1);
			break ;
		}
		root = token;
	}
}

void	ServerBlock::reset( void ) {
	listen.clear();
	server_name.clear();
	root.clear();
	client_max_body_size = ULONG_MAX;
	index.clear();
	autoindex = false;
	error_page.clear();
	location.clear();
}

void	ServerBlock::addListen( const Socket &add ) { listen.push_back(add); }

void	ServerBlock::addServerName( const std::string &add ) { server_name = add; }

void	ServerBlock::addRoot( const std::string &add ) { root = add; }

void	ServerBlock::setClientMax( uint64_t add ) { client_max_body_size = add; }

void	ServerBlock::addIndex( const std::string &add ) { index = add; }

void	ServerBlock::toggleAutoIndex( void ) { autoindex = true; }

void	ServerBlock::addErrorPage( uint16_t error_code, const std::string &path ) {
	error_page[error_code] = path;
}

std::string	ServerBlock::testHTML( void ) {
	std::string	msg = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	std::string	html = "<html><h1>Hello world</h1></html?";
	msg.append(to_String(html.size()));
	msg.append("\n\n");
	msg.append(html);
	return (msg);
}

bool	ServerBlock::checkDupSocket( const Socket &ref ) {
	typedef std::vector<Socket>::iterator iter;
	for (iter it = listen.begin(); it != listen.end(); it ++) {
		if (*it == ref)
			return (true);
	}
	return (false);
}

std::ostream&	operator<<( std::ostream &o, const ServerBlock &ref ) {
	typedef std::vector<Socket>::const_iterator sock_iter;
	typedef std::map<uint16_t, std::string>::const_iterator error_iter;
	o << "Listening on: ";
	for (sock_iter it = ref.listen.begin(); it != ref.listen.end(); it ++) {
		o << *it << ' ';
	}
	o << '\n';
	o << "Server name: " << ref.server_name << '\n';
	o << "Root directory: " << ref.root << '\n';
	o << "Client max body size: " << ref.client_max_body_size << '\n';
	o << "Index: " << ref.index << '\n';
	o << "Error pages: ";
	for (error_iter it = ref.error_page.begin(); it != ref.error_page.end(); it ++) {
		o << it->first << " = " << it->second << ' ';
	}
	o << '\n';
	return (o);
}
