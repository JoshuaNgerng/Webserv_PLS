/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExcept.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 16:46:13 by joshua            #+#    #+#             */
/*   Updated: 2024/11/14 21:52:48 by jngerng          ###   ########.fr       */
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
