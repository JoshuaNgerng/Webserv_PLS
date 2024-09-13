/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:09:04 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/21 21:25:45 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerBlock.hpp"

ServerBlock::ServerBlock( void ) : listen(), server_name(),
	root(), access_log(), error_log(), ssl_certificate(),
	ssl_certificate_key() ,index(), hostname(), client_max_body_size(ULONG_MAX),
	autoindex(false), error_page() { }

ServerBlock::ServerBlock( const ServerBlock &src ) : listen(src.listen), server_name(src.server_name),
	root(src.root), access_log(src.access_log), error_log(src.error_log), ssl_certificate(src.ssl_certificate),
	ssl_certificate_key(src.ssl_certificate_key) ,index(src.index), hostname(src.hostname), client_max_body_size(src.client_max_body_size),
	autoindex(src.autoindex), error_page(src.error_page) { }

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

void	ServerBlock::setClientMax( uint64_t add ) { client_max_body_size = add; }

void	ServerBlock::addIndex( const std::string &add ) { index = add; }

void	ServerBlock::toggleAutoIndex( void ) { autoindex = !autoindex; }

void	ServerBlock::addErrorPage( uint16_t error_code, const std::string &path ) {
	error_page[error_code] = path;
}

void	ServerBlock::addListen( const Socket &add ) { listen.push_back(add); }

void	ServerBlock::addLocation( const Location &add )
{
	// location.push_back(add);
	location.push_back(Location(add));

}

void	ServerBlock::addServerName( const std::string &add ) { server_name = add; }

void	ServerBlock::addRoot( const std::string &add ) { root = add; }

void	ServerBlock::addAccessLog( const std::string &add ) { access_log = add; }

void	ServerBlock::addErrorLog( const std::string &add ) { error_log = add; }

void	ServerBlock::addSSLCertificate( const std::string &add ) { ssl_certificate = add; }

void	ServerBlock::addSSLCertificateKey( const std::string &add ) { ssl_certificate_key = add; }

// void	ServerBlock::addClientLimit( const std::string &add ) { client_limit = add; }

void	ServerBlock::addHostname( const std::string &add ) { hostname = add; }

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
	if (ref.error_page.begin() == ref.error_page.end())
		return (o);
	for (error_iter it = ref.error_page.begin(); it != ref.error_page.end(); it ++) {
		o << it->first << " = " << it->second << ' ';
	}
	o << '\n';
	return (o);
}
