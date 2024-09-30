/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 01:46:51 by joshua            #+#    #+#             */
/*   Updated: 2024/09/28 06:12:19 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP
# include "Const.hpp"

// int for size of requ
// int for size of req field
// int for clientbodymax
class Http {
	public:
		enum http_method { GET, POST, PUT, DELETE };
		enum field {
			HOST, USER_AGENT,
			ACCEPT, LANG, ENCODE,
			CONNECTION, REFERER, COOKIE, CACHE,
			C_TYPE, C_LEN
		};
		enum type {
			TEXT, HTML, JS, CSS,
			JPEG, PNG
		};
		Http( void );
		Http( const Http &src );
		virtual ~Http( void );

	protected:
		static const char *const *methods;

		int	checkMethods( const std::string &str ) const;
		int	checkField( const std::string &str ) const;
		int	checkType( const std::string &str ) const;

		template < typename E >
		int	iterEnum( E start, E end, int val ) const {
			for (int i = static_cast<int>(start);
				 i <= static_cast<int>(end); i ++) {
				if (val == i) {
					return (i);
				}
			}
			return (-1);
		}

		template < typename E >
		int	iterEnumStrcmp( E start, E end,
			const std::string &str, const char *const *ref ) const {
			for (int i = static_cast<int>(start);
				 i <= static_cast<int>(end); i ++) {
				if (str == ref[i]) {
					return (i);
				}
			}
			return (-1);	
		}

	private:
		static const char *const *fields;
		static const char *const *types;

};

#endif
