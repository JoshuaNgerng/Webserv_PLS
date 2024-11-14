/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExcept.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 22:45:19 by joshua            #+#    #+#             */
/*   Updated: 2024/11/15 00:40:26 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LimitExcept.hpp"
#include "Client.hpp"

LimitExcept::LimitExcept( void ) : 
network_ranges_v4(),
network_ranges_v6(),
methods()
{ }

LimitExcept::LimitExcept( const LimitExcept &src ) { *this = src; }

LimitExcept::~LimitExcept( void ) { }

LimitExcept&	LimitExcept::operator=( const LimitExcept &src ) {
	if (this == &src) {
		return (*this);
	}
	network_ranges_v4 = src.network_ranges_v4;
	network_ranges_v6 = src.network_ranges_v6;
	methods = src.methods;
	return (*this);
}

void	LimitExcept::addMethods( const std::string& method ) {
	int check = Http::checkMethods(method);
	if (check < 0) {
		throw std::invalid_argument("LimitExcept method");
	}
	methods.push_back(static_cast<Http::http_method>(check));
}

static addrinfo_t*	getAddrInfo( const std::string& addr ) {
	addrinfo_t	*res = NULL;
	addrinfo_t	hints = {0, AF_UNSPEC, SOCK_STREAM, 0, sizeof(*(hints.ai_addr)), 0, 0, 0};
	int status = 0;
	if ((status = getaddrinfo(addr.c_str(), NULL, &hints, &res)) < 0) {
		return (NULL);
	}
	return (res);
}

static addrinfo_t*	getAddrInfo( void ) {
	addrinfo_t	*res = NULL , *ptr = NULL;
	addrinfo_t	hints = {0, AF_UNSPEC, SOCK_STREAM, 0, sizeof(*(hints.ai_addr)), 0, 0, 0};
	int status = 0;
	if ((status = getaddrinfo("0.0.0.0", NULL, &hints, &res)) < 0) {
		return (NULL);
	}
	ptr = res;
	while (ptr->ai_next) {
		ptr = ptr->ai_next;
	}
	if ((status = getaddrinfo("::", NULL, &hints, &ptr->ai_next)) < 0) {
		freeaddrinfo(res);
		return (NULL);
	}
	return (res);
}

void	LimitExcept::addNetworkHelper(
			NetworkMapping &dst, bool cond, bool status, int len, int limit,
			const addrinfo_t *ptr
) {
	if (!status) {
		len = limit;
	} else {
		if (len > limit) {
			throw std::invalid_argument("LimitExcept prefix too large");
		}
	}
	dst.insert(std::pair<NetworkRange, bool>(NetworkRange(ptr, len), cond));
}

void	LimitExcept::addNetwork( const std::string &str, bool cond ) {
	addrinfo_t	*addr = NULL;
	int			len = 0;
	bool		status = false;
	if (str == "all") {
		addr = getAddrInfo();
		status = false;
	} else {
		size_t pos = str.find('/');
		if (pos != std::string::npos) {
			status = true;
			len = std::atoi(str.c_str() + pos + 1);
		}
		addr = getAddrInfo(str);
	}
	if (!addr) {
		throw std::invalid_argument("LimitExcept allow");
	}
	for (const addrinfo_t *ptr = addr; ptr != NULL; ptr = ptr->ai_next) {
		if (ptr->ai_family == AF_INET) {
			addNetworkHelper(network_ranges_v4, cond, status, len, 32, ptr);
		} else {
			addNetworkHelper(network_ranges_v6, cond, status, len, 128, ptr);
		}
	}
	freeaddrinfo(addr);
}

void	LimitExcept::addAllow( const std::string& str ) {
	addNetwork(str, true);
}

void	LimitExcept::addDeny( const std::string& str ) {
	addNetwork(str, false);
}

void	LimitExcept::reset( void ) {
	network_ranges_v4.clear();
	network_ranges_v6.clear();
	methods.clear();
}
size_t	LimitExcept::getNumMethod( void ) const { return (methods.size()); }

int	LimitExcept::checkAccess( const Client &client ) const {
	typedef std::map<NetworkRange, bool>					mapping;
	typedef std::map<NetworkRange, bool>::const_iterator	iter;
	typedef std::vector<Http::http_method>::const_iterator	method_iter;
	if (!methods.size()) {
		return (200);
	}
	const sockaddr_storage_t& addr = client.getAddr(*this);
	bool	method_allowed = false;
	bool	within_range = false;
	bool	allow_or_deny = true;
	for (method_iter it = methods.begin(); it != methods.end(); it ++) {
		if (*it == client.getReqMethod(*this)) {
			method_allowed = true;
			break ;
		}
	}
	const mapping	*ptr = &network_ranges_v4;
	if (addr.ss_family == AF_INET6) {
		ptr = &network_ranges_v6;
	}
	for (iter it = ptr->begin(); it != ptr->end(); it ++) {
		if (it->first.isWithinRange(addr)) {
			within_range = true;
			allow_or_deny = it->second;
			break ;
		}
	}
	if (method_allowed && within_range) {
		return (200);
	}
	if (!method_allowed && within_range && allow_or_deny) {
		return (405);
	}
	return (403);
}

LimitExcept::NetworkRange::NetworkRange( void ) { }

static void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t i = -1;
	if (!dst && !src)
		return (0);
	while (++ i < n) {
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
	}
	return (dst);
}

LimitExcept::NetworkRange::NetworkRange( const addrinfo_t *addr_info, uint8_t prefix_ ) {
	prefix_length = prefix_;
	sa_family = addr_info->ai_family;
	for (int i = 0; i < 16; i ++) {
		bytes[i] = 0;
	}
	if (sa_family == AF_INET) {
		struct sockaddr_in* ipv4 = (struct sockaddr_in*)addr_info->ai_addr;
		ft_memcpy(bytes, &ipv4->sin_addr, sizeof(ipv4->sin_addr));
	} else {
		struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)addr_info->ai_addr;
		ft_memcpy(bytes, &ipv6->sin6_addr, sizeof(ipv6->sin6_addr)); 
	}
}

LimitExcept::NetworkRange::NetworkRange( const NetworkRange &src ) { *this = src; }

LimitExcept::NetworkRange::~NetworkRange( void ) { }

LimitExcept::NetworkRange&	LimitExcept::NetworkRange::operator=( const NetworkRange &src ) {
	if (this == &src) {
		return (*this);
	}
	prefix_length = src.prefix_length;
	sa_family = src.sa_family;
	for (int i = 0; i < 16; i ++) {
		bytes[i] = src.bytes[i];
	}
	return (*this);
}
bool	LimitExcept::NetworkRange::operator<( const NetworkRange &src ) const {
	return (prefix_length > src.prefix_length);
}

bool	LimitExcept::NetworkRange::isWithinRangev4( const sockaddr_storage_t& addr ) const {
	// For IPv4, cast the sockaddr_storage to sockaddr_in
	const sockaddr_in* ipv4 = reinterpret_cast<const sockaddr_in*>(&addr);
	uint32_t ip_addr = ntohl(ipv4->sin_addr.s_addr);  // Convert to host byte order

	// Convert network address to uint32_t for comparison
	uint32_t network_addr = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];

	// special case for 0.0.0.0 since it will always be 0
	if (network_addr == 0 && prefix_length == 32) {
		return (true); // 0.0.0.0/32 matches all IP addresses
	}

	// Create subnet mask
	uint32_t mask = (0xFFFFFFFF << (32 - prefix_length)) & 0xFFFFFFFF;

	return ((ip_addr & mask) == (network_addr & mask));
}

bool	LimitExcept::NetworkRange::isWithinRangev6( const sockaddr_storage_t& addr ) const {
	// For IPv6, cast the sockaddr_storage to sockaddr_in6
	const sockaddr_in6* ipv6 = reinterpret_cast<const sockaddr_in6*>(&addr);

	if (prefix_length == 128) {
		return (true);
	}
	// Compare byte by byte for IPv6 address (128 bits)
	int full_bytes = prefix_length / 8;
	int partial_byte = prefix_length % 8;

	// Check full bytes
	for (int i = 0; i < full_bytes; ++i) {
		if (ipv6->sin6_addr.s6_addr[i] != bytes[i]) {
			return (false);
		}
	}

	// Check partial byte
	if (partial_byte > 0) {
		uint8_t mask = (0xFF << (8 - partial_byte)) & 0xFF;
		if ((ipv6->sin6_addr.s6_addr[full_bytes] & mask) != (bytes[full_bytes] & mask)) {
			return (false);
		}
	}
	return (true);
}

bool	LimitExcept::NetworkRange::isWithinRange( const sockaddr_storage_t& addr ) const {
	if (sa_family == AF_INET) {
		return (isWithinRangev4(addr));
	}
	return (isWithinRangev6(addr));
}
