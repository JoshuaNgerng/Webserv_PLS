/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:25:21 by joshua            #+#    #+#             */
/*   Updated: 2024/11/13 19:06:38 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include "Http.hpp"

class HttpRequest : public Http {
	enum error_type {
		malform_header = 1,
		limit_exceed = 2
	};
	public:
		HttpRequest( void );
		HttpRequest( size_t header_size );
		HttpRequest( const HttpRequest &src );
		HttpRequest& operator=( const HttpRequest &src );
		~HttpRequest( void );

		const std::string&	getUrl( void ) const;
		const std::string&	getUri( void ) const;
		const std::string&	getPath( void ) const;
		const std::string&	getQuery( void ) const;
		const std::string&	getProtocol( void ) const; 

		size_t		addRequest( const std::string &str );
		http_method	getMethod( void ) const;
		bool		getHasBody( void ) const;
		int			getValidHeader( void ) const;

		const std::string&	getContentType( void ) const;

		uint64_t	getContentLength( void ) const;
		void		normalizeUri( void );
		size_t		addHeader( const std::string &str, size_t bytes );
		size_t		addBody( const std::string &str, size_t pos );

	private:
		size_t		header_limit;
		short		error;
		http_method	method;
		std::string	url;
		std::string	uri;
		std::string	path; // decode?
		std::string	query; // decode?
		std::string	protocol;
		bool		has_body;

		bool	validateStartLine( const std::string &start );
		bool	validateHeader( void );
		bool	validateUrl( void );
		bool	validateBody( void );
		bool	addHeaderFields( const std::string &field, const std::string &val );
};

std::ostream&	operator<<( std::ostream &o, const HttpRequest &res );

#endif
