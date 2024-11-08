/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:25:21 by joshua            #+#    #+#             */
/*   Updated: 2024/11/07 18:53:02 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP
# include "Http.hpp"

class HttpRequest : public Http {
	enum error_type {
		malform_header = 1,
		limit_exceed = 2
	};
	public:
		HttpRequest( void );
		HttpRequest( const std::string &str, size_t pos );
		HttpRequest( const HttpRequest &src );
		HttpRequest& operator=( const HttpRequest &src );
		~HttpRequest( void );
		size_t		addRequest( const std::string &str );
		http_method	getMethod( void ) const;
		const std::string&	getUrl( void ) const;
		const std::string&	getUri( void ) const;
		const std::string&	getPath( void ) const;
		const std::string&	getQuery( void ) const;
		const std::string&	getProtocol( void ) const; 
		const std::string&	getHeaderStr( void ) const;
		const std::string&	getField( const char *field ) const;
		bool		getHasBody( void ) const;
		int			getValidHeader( void ) const;

		std::string	getContentType( void ) const;
		uint64_t	getContentLength( void ) const;
		void		normalizeUri( void );
		size_t		addHeader( const std::string &str, size_t bytes );
		size_t		addBody( const std::string &str, size_t pos );

	private:
		static const size_t	header_limit = 10000;
		std::map<std::string, std::string>	header_fields;

		short		error;
		http_method	method;
		std::string	url;
		std::string	uri;
		std::string	path;
		std::string	query;
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
