/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExcept.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 16:46:13 by joshua            #+#    #+#             */
/*   Updated: 2024/11/14 17:50:51 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIMITEXCEPT_HPP
# define LIMITEXCEPT_HPP
# include "Http.hpp"

class LimitExcept {
	public:
		LimitExcept( void );
		LimitExcept( const LimitExcept &src );
		~LimitExcept( void );

		LimitExcept&	operator=( const LimitExcept &src );

		void	addMethods( const std::string& method );
		void	addAllow( const std::string& str );
		void	addDeny( const std::string& str );
		void	reset( void );

		size_t	getNumMethod( void ) const;

		int 	checkAccess( const Client &client ) const;


	private:
		class NetworkRange {
			public:
				NetworkRange( const addrinfo_t *addr_info, uint8_t prefix_ );
				NetworkRange( const NetworkRange &src );
				~NetworkRange( void );

				NetworkRange&	operator=( const NetworkRange &src );
				bool			operator<( const NetworkRange &src ) const;

				bool			isWithinRange( const sockaddr_storage& addr ) const;

			private:
				uint8_t	prefix_length;
				uint8_t	sa_family;
				uint8_t	bytes[16];

				NetworkRange( void );
				bool			isWithinRangev4( const sockaddr_storage& addr ) const;
				bool			isWithinRangev6( const sockaddr_storage& addr ) const;
		};

		typedef std::map<NetworkRange, bool>	NetworkMapping;
		NetworkMapping							network_ranges_v4;
		NetworkMapping							network_ranges_v6;
		std::vector<Http::http_method>			methods;

		void	addNetwork( const std::string &str, bool cond );
		void	addNetworkHelper(
			NetworkMapping &dst, bool cond, bool status,
			int len, int limit, const addrinfo_t *ptr
		);
};

#endif

/*
#include <iostream>
#include <cstring>
#include <arpa/inet.h>

class LimitExcept {
private:
	struct byte_rep {
		uint8_t bytes[16];  // This will hold both IPv4 and IPv6 addresses
	};

	byte_rep network_add;  // Network address
	int sa_family;         // AF_INET (IPv4) or AF_INET6 (IPv6)
	int prefix_len;        // Prefix length (CIDR notation)

public:
	// Constructor to initialize the LimitExcept object with network info
	LimitExcept(const std::string& network, int prefix_length, int family) {
		sa_family = family;
		prefix_len = prefix_length;
		if (sa_family == AF_INET) {
			struct in_addr in;
			inet_pton(AF_INET, network.c_str(), &in);
			std::memcpy(network_add.bytes, &in, sizeof(in));  // Store IPv4 in the first 4 bytes
		} else if (sa_family == AF_INET6) {
			struct in6_addr in6;
			inet_pton(AF_INET6, network.c_str(), &in6);
			std::memcpy(network_add.bytes, &in6, sizeof(in6));  // Store full 16-byte IPv6 address
		}
	}

	// Helper function to check if a given IP address falls within the network range
	bool is_within_range(const sockaddr_storage& addr) const {
		if (sa_family == AF_INET) {
			// For IPv4, cast the sockaddr_storage to sockaddr_in
			const sockaddr_in* ipv4 = reinterpret_cast<const sockaddr_in*>(&addr);
			uint32_t ip_addr = ntohl(ipv4->sin_addr.s_addr);  // Convert to host byte order

			// Convert network address to uint32_t for comparison
			uint32_t network_addr = (network_add.bytes[0] << 24) |
									 (network_add.bytes[1] << 16) |
									 (network_add.bytes[2] << 8) |
									 network_add.bytes[3];

			// Create subnet mask
			uint32_t mask = (0xFFFFFFFF << (32 - prefix_len)) & 0xFFFFFFFF;

			return (ip_addr & mask) == (network_addr & mask);
		} else if (sa_family == AF_INET6) {
			// For IPv6, cast the sockaddr_storage to sockaddr_in6
			const sockaddr_in6* ipv6 = reinterpret_cast<const sockaddr_in6*>(&addr);

			// Compare byte by byte for IPv6 address (128 bits)
			int full_bytes = prefix_len / 8;
			int partial_byte = prefix_len % 8;

			// Check full bytes
			for (int i = 0; i < full_bytes; ++i) {
				if (ipv6->sin6_addr.s6_addr[i] != network_add.bytes[i]) {
					return false;
				}
			}

			// Check partial byte
			if (partial_byte > 0) {
				uint8_t mask = (0xFF << (8 - partial_byte)) & 0xFF;
				if ((ipv6->sin6_addr.s6_addr[full_bytes] & mask) != (network_add.bytes[full_bytes] & mask)) {
					return false;
				}
			}

			return true;
		}

		return false;  // Unknown address family
	}
};

int main() {
	// Example usage
	// Create a LimitExcept object for the 192.168.1.0/24 network (IPv4)
	LimitExcept limit_except("192.168.1.0", 24, AF_INET);

	// Create a sockaddr_storage with an example IP (192.168.1.10)
	sockaddr_in sa_in;
	sa_in.sin_family = AF_INET;
	inet_pton(AF_INET, "192.168.1.10", &sa_in.sin_addr);

	sockaddr_storage sa_storage;
	std::memcpy(&sa_storage, &sa_in, sizeof(sa_in));

	// Check if the IP is within the range
	if (limit_except.is_within_range(sa_storage)) {
		std::cout << "The address is within the range.\n";
	} else {
		std::cout << "The address is NOT within the range.\n";
	}

	return 0;
}

*/