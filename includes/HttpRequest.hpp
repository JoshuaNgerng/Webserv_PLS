/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:25:21 by joshua            #+#    #+#             */
/*   Updated: 2024/11/05 18:06:28 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP
# include "Http.hpp"

class HttpRequest : public Http {
	enum error_type {
		malform_header = 1,
		request_too_large = 2,
		limit_exceed = 4
	};
	public:
		HttpRequest( void );
		HttpRequest( const std::string &str, size_t pos );
		HttpRequest( const HttpRequest &src );
		HttpRequest& operator=( const HttpRequest &src );
		~HttpRequest( void );
		size_t		addRequest( const std::string &str );
		http_method	getMethod( void ) const;
		const std::string&	getUri( void ) const;
		const std::string&	getProtocol( void ) const; // empty
		const std::string&	getHeaderStr( void ) const; //empty
		bool		getHasBody( void ) const;
		int			getValidHeader( void ) const;

		type		getContentType( void ) const;
		uint64_t	getContentLength( void ) const;
		void		normalizeUri( void );
		size_t		addHeader( const std::string &str, size_t bytes );
		size_t		addBody( const std::string &str, size_t pos );

	private:
		static const size_t	header_field_limit = 1000;
		static const size_t	header_limit = 10000;
		static const size_t	body_limit = 150000;
		std::map<std::string, std::string>	header_fields;

		short		error;
		http_method	method;
		std::string	uri;
		std::string	protocol;
		bool		has_body;

		bool	validateStartLine( const std::string &start );
		bool	validateHeader( void );
		bool	validateHeaderHelper( void );
		bool	validateBody( void );
		bool	addHeaderFields( const std::string &field, const std::string &val );
};

std::ostream&	operator<<( std::ostream &o, const HttpRequest &res );

#endif
