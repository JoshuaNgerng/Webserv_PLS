/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:25:21 by joshua            #+#    #+#             */
/*   Updated: 2024/10/31 07:56:49 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP
# include "Http.hpp"

class HttpRequest : Http {
	public:
		HttpRequest( void );
		HttpRequest( const std::string &str, size_t pos );
		HttpRequest( const HttpRequest &src );
		HttpRequest& operator=( const HttpRequest &src );
		~HttpRequest( void );
		size_t		addRequest( const std::string &str );
		size_t		addBody( const std::string &str, size_t pos );
		http_method	getMethod( void ) const;
		const std::string&	getUri( void ) const;
		const std::string&	getProtocol( void ) const; // empty
		const std::string&	getHeaderStr( void ) const; //empty
		bool		getHasBody( void ) const;
		bool		getValidHeader( void ) const;
		bool		isReady( void ) const; //empty

		type		getContentType( void ) const;
		uint64_t	getContentLength( void ) const;
		void		normalizeUri( void );

	private:
		std::string	header;
		std::string	body;
		std::map<std::string, std::string>	header_fields;

		bool		valid_header;
		http_method	method;
		std::string	uri;
		std::string	protocol;
		bool		has_body;
		type		content_type;
		uint64_t	content_length;
		bool		finished_request;

		bool	validateStartLine( const std::string &start );
		bool	validateHeader( void );
		bool	validateHeaderHelper( void );
		bool	validateBody( void );
		bool	addHeaderFields( const std::string &field, const std::string &val );
};

std::ostream&	operator<<( std::ostream &o, const HttpRequest &res );

#endif
