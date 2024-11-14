/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 01:46:51 by joshua            #+#    #+#             */
/*   Updated: 2024/11/14 12:24:43 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP
# include "Const.hpp"

// int for size of requ
// int for size of req field
// int for clientbodymax
class Http {
	typedef std::map<std::string, std::string> mapping;
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

		static const char	*fields[];

		virtual ~Http( void );

		static const char	*fetchMsg( int status );
		static const char	*getMimeType( const std::string &ext );

		bool				validateField( std::string &field ) const;
		bool				validateValue( const std::string &val ) const;
		bool				addHeaderFields( const std::string &field, const std::string &val );

		std::string&		modifyHeader( void );
		std::string&		modifyBody( void );

		virtual void		finishHttp( void );
		size_t				getHeaderLength( void ) const;
		size_t				getBodyLength( void ) const;
		size_t				getTotalLength( void ) const;
		size_t				getRemainderHeader( void ) const;
		size_t				getRemainderBody( void ) const;
		size_t				getRemainderHttp( void ) const;
		size_t				getContentLength( void ) const;
		const char*			getPtr2Header( void ) const;
		const char*			getPtr2Body( void ) const;
		const char*			getPtr2Http( void ) const;
		const std::string&	getHeader( void ) const;
		const std::string&	getBody( void ) const;
		const std::string&	getField( const char *str ) const;
		bool				isHeaderReady( void ) const;
		bool				isReady( void ) const;
		bool				checkSendHeader( size_t bytes = 0 );
		bool				checkSendBody( size_t bytes = 0 );
		bool				checkSendHttp( size_t bytes = 0 );
		void				reset( void );

		static int	checkMethods( const std::string &str );
		static int	checkField( const std::string &str );

	protected:
		Http( void );
		Http( const Http &src );
		Http&	operator=( const Http &src );
		static const char	*methods[];
		bool				header_ready;
		bool				ready;
		std::string			header;
		std::string			body;
		std::string			combine;
		size_t				bytes_sent;
		size_t				content_length;
		std::string			content_type;
		mapping				header_fields;

	
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
		static int	iterEnumStrcmp( E start, E end,
			const std::string &str, const char *const *ref ) {
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

		bool	checkSend( size_t bytes, size_t len );
};

#endif
