/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 01:46:51 by joshua            #+#    #+#             */
/*   Updated: 2024/11/07 18:17:39 by jngerng          ###   ########.fr       */
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
		typedef struct code_msg_pair {
			int			status;
			const char	*msg;
		}	t_pairs;
		typedef struct mime_type_pair {
			const char	*extension;
			const char	*mime_type;
		}	t_types;
		enum http_method { GET, POST, PUT, DELETE };
		enum field {
			HOST, USER_AGENT,
			ACCEPT, LANG, ENCODE,
			CONNECTION, REFERER, COOKIE, CACHE,
			C_TYPE, C_LEN
		};
		enum type {
			TEXT_PLAIN, HTML, JS, CSS,
			JPEG, PNG, OCTECT
		};
		virtual ~Http( void );

		static const char	*fetchMsg( int status );
		static const char	*getMimeType( const std::string &ext );

		// virtual size_t	addHeader( const std::string &str );
		// virtual size_t	addBody( const std::string &str );
		// virtual size_t	addBody( const char *str, size_t bytes );
		virtual void	finishHttp( void );
		const char*		getPtr2Http( size_t bytes = 0 ) const;
		bool			isReady( void ) const;
		void			reset( void );

	protected:
		Http( void );
		Http( const Http &src );
		Http&	operator=( const Http &src );
		static const char	*methods[];
		static const char	*fields[];
		bool				ready;
		std::string			header;
		std::string			body;
		std::string			combine;
		size_t				content_length;
		type				content_type;

		int	checkMethods( const std::string &str ) const;
		int	checkField( const std::string &str ) const;
	
		bool	makeReponseHeader( std::string &str, int status ) const;

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
		static const t_pairs 		pairing[];
		static const t_types		mime_types[];
};

#endif
