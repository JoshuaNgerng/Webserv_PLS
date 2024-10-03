/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:44:50 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/30 17:16:54 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ListenSocket.hpp"

int	ListenSocket::fcntl_flag = O_NONBLOCK;

ListenSocket::ListenSocket( void ) { addr_info_tail = &addr_info_head; }

// ListenSocket::ListenSocket( const ListenSocket &src ) :
// 	addr_info_head(src.addr_info_head), addr_info_tail(src.addr_info_tail) { }

ListenSocket::ListenSocket( const ListenSocket &src ) {
	*this = src; 
}

ListenSocket& ListenSocket::operator=(const ListenSocket &other) {
    if (this != &other) {
        addr_info_head = other.addr_info_head;
        addr_info_tail = other.addr_info_tail;
        default_server = other.default_server;
        backlog = other.backlog;
        rcvbuf_size = other.rcvbuf_size;
        sndbuf_size = other.sndbuf_size;
        ipv6only = other.ipv6only;
        reuseport = other.reuseport;
        keepalive = other.keepalive;
        keepidle = other.keepidle;
        keepintvl = other.keepintvl;
        keepcnt = other.keepcnt;
        len = other.len;
        status = other.status;
    }
    return *this;
}

ListenSocket::~ListenSocket( void ) { freeaddrinfo(addr_info_head); }

ListenSocket&	ListenSocket::operator=( const ListenSocket &src )
{ addr_info_head = src.addr_info_head; addr_info_tail = src.addr_info_tail; return (*this); }

bool	ListenSocket::addAddress(
	const std::string &addr, const std::string &port = std::string()
) {
	addrinfo_t	hints = {(AI_CANONNAME | AI_PASSIVE), AF_UNSPEC, SOCK_STREAM,
							0, sizeof(*(hints.ai_addr)), 0, 0, 0};
	if (ipv6only)
		hints.ai_family = AF_INET6;
	if ((status = getaddrinfo(addr.c_str(),
			((port.length()) ? port.c_str() : "80"), &hints, addr_info_tail)) < 0)
		return (false);
	addrinfo_t	*ptr = *addr_info_tail;
	while (ptr->ai_next) {
		len ++;
		ptr = ptr->ai_next;
	}
	addr_info_tail = &(ptr->ai_next);
	len ++;
	return (true);
}

int	ListenSocket::addListenPollFd( std::vector<pollfd_t> &listen_v ) const {
	uint32_t	num = 0;
	pollfd_t	pollfd;
	pollfd.fd = -1;
	pollfd.events = POLLIN;
	pollfd.revents = 0;
	for (Iterator it = this->begin(); it != this->end(); it ++) {
		pollfd.fd = addListenFd(it);
		if (pollfd.fd < 0) {
			return (-1);
		}
		listen_v.push_back(pollfd);
		num ++;
	}
	return (num);
}

bool	ListenSocket::socketSetup( int fd ) const {
	int	opt = 1;
	if (fcntl(fd, F_SETFL, fcntl_flag) < 0)
	if (reuseport) {
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEADDR,
			&opt, sizeof(opt)) < 0) {
			return (false);
		}
	}
	if (sndbuf_size > 0) {
		if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF,
			&sndbuf_size, sizeof(sndbuf_size)) < 0) {
			return (false);
		}
	}
	if (rcvbuf_size > 0) {
		if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF,
			&rcvbuf_size, sizeof(rcvbuf_size)) < 0) {
			return (false);
		}
	}
	if (!keepalive)
		return (true);
	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) < 0) {
		return (false);
	}
	if (keepcnt > 0) {
		if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT,
			&keepcnt, sizeof(keepcnt)) < 0) {
			return (false);
		}
	}
	if (keepintvl > 0) {
		if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL,
			&keepintvl, sizeof(keepintvl)) < 0) {
			return (false);
		}
	}
	if (keepidle <= 0)
		return (true);
	#ifdef __APPLE__
	if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPALIVE, &keepidle, sizeof(keepidle)) < 0) {
		return (false);	
	}
	#else
	if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle)) < 0) {
		return (false);	
	}
	#endif
	return (true);
}

int	ListenSocket::addListenFd( const Iterator &it ) const {
	int	fd = socket(it->ai_family, it->ai_socktype, it->ai_protocol);
	if (fd < 0) {
		return (-1);
	}
	if (!socketSetup(fd)) {
		return (-1);
	}
	if (bind(fd, it->ai_addr, it->ai_addrlen) < 0) {
		return (-1);
	}
	if (listen(fd, backlog) < 0) {
		return (-1);
	}
	return (fd);
}

void	ListenSocket::setDefaultServer( void ) { default_server = true; }

void	ListenSocket::setBackLog( uint64_t num ) {
	uint16_t	check = static_cast<uint16_t>(num);
	if (check > SOMAXCONN)
		throw std::invalid_argument("backlog number too big");
	backlog = check;
}

void	ListenSocket::setRcvBuf( uint64_t num ) { rcvbuf_size = num; }

void	ListenSocket::setSndBuf( uint64_t num ) { sndbuf_size = num; }

void	ListenSocket::setIpv6( void ) { ipv6only = true; }

void	ListenSocket::setReusePort( void ) { reuseport = true; }

void	ListenSocket::setKeepalive( void ) { keepalive = true; }

void	ListenSocket::setKeepalive( long idle, long intvl, long cnt ) {
	keepalive = true;
	if (idle > 0)
		keepidle = idle;
	if (intvl > 0)
		keepintvl = intvl;
	if (cnt > 0)
		keepcnt = cnt;
}

void	ListenSocket::clear( void ) {
	addr_info_tail = &addr_info_head;
	if (!addr_info_head)
		return ;
	freeaddrinfo(addr_info_head);
	addr_info_head = NULL; 
}

void	ListenSocket::reset( void ) {
	clear();
	default_server = false;
	backlog = SOMAXCONN;
	rcvbuf_size = 65536;
	sndbuf_size = 65536;
	ipv6only = false;
	reuseport = false;
	keepalive = false;
	keepidle = -1;
	keepintvl = -1;
	keepcnt = -1;
	len = 0;
	status = 0;
}

ListenSocket::Iterator	ListenSocket::begin( void ) const {
	return (Iterator(addr_info_head));
}

ListenSocket::Iterator	ListenSocket::end( void ) const { return (Iterator()); }

uint32_t	ListenSocket::length( void ) const { return (len); }

int	ListenSocket::getStatus( void ) const { return (status); }

ListenSocket::Iterator::Iterator( void ) : ptr(0) { }

ListenSocket::Iterator::Iterator( const Iterator &src ) : ptr(src.ptr) { }

ListenSocket::Iterator& ListenSocket::Iterator::operator=( const Iterator &src ) {
	ptr = src.ptr; return (*this);
}

ListenSocket::Iterator::Iterator( addrinfo_t *start ) : ptr(start) { }

ListenSocket::Iterator::~Iterator( void ) { }

ListenSocket::Iterator&	ListenSocket::Iterator::operator=( const Iterator &src ) {
	ptr = src.ptr; return (*this);
}

addrinfo_t&	ListenSocket::Iterator::operator*( void ) { return (*ptr); }

addrinfo_t*	ListenSocket::Iterator::operator->( void ) const { return (ptr); }

ListenSocket::Iterator&	ListenSocket::Iterator::operator++( void ) {
	ptr = ptr->ai_next; return (*this);
}

ListenSocket::Iterator	ListenSocket::Iterator::operator++( int ) {
	Iterator temp(*this);
	++(*this);
	return temp;
}

ListenSocket::Iterator&	ListenSocket::Iterator::operator+( size_t n ) {
	for (size_t i = 0; i < n; i ++) {
		++(*this);
	}
	return (*this);
}

bool	ListenSocket::Iterator::operator!=( const Iterator& other ) const {
	return (ptr != other.ptr);
}

bool	ListenSocket::Iterator::operator==( const Iterator& other ) const {
	return (ptr == other.ptr);
}

std::ostream&	operator<<( std::ostream &o, const ListenSocket& ref ) {
	char	buffer[INET6_ADDRSTRLEN];
	for(ListenSocket::Iterator it = ref.begin(); it != ref.end(); it ++) {
		void	*addr;
		int		port;
		if (it->ai_family == AF_INET) {
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)it->ai_addr;
			addr = &(ipv4->sin_addr);
			port = ntohs(ipv4->sin_port);
		} else {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)it->ai_addr;
			addr = &(ipv6->sin6_addr);
			port = ntohs(ipv6->sin6_port);
		}
		inet_ntop(it->ai_family, addr, buffer, sizeof(buffer));
		std::cout << buffer << ':' << port << ' ';
	}
	return (o);
}
