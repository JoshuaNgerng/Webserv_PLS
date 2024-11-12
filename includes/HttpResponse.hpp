/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 06:20:04 by joshua            #+#    #+#             */
/*   Updated: 2024/11/12 18:19:34 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP
# include "Http.hpp"

class HttpResponse : public Http {
	public:
		HttpResponse( void );
		HttpResponse( const HttpResponse &src );
		HttpResponse( bool proxy_ );
		~HttpResponse( void );
		HttpResponse&	operator=( const HttpResponse &src );

		void	addFin( const std::string &str );
		void	addFin( const char *str, size_t bytes );
		void	addBody( const std::string &str );
		void	addBody( const char *str, size_t bytes );
		void	setHeader( int status, const std::string & = std::string() );
		void	setContent( const std::string &type, uint64_t len );
		void	setContent( void );
		void	reset( void );

		bool	isReady( void ) const;
		bool	processCgiData( void );

	private:
		int		status;
		bool	proxy;

		bool	validateHttpStart( const std::string &line ) const;
		bool	generateHeader( const std::string &buffer );
		void	addHeaderStart( int status );
		void	addHeader( void );
		void	addHeader( int status );
		void	addField( const char *name, const std::string &val );
		void	addField( const char *name, const char *val );
		void	addField( const std::string &name, const std::string &val );
		void	addField( std::string &str, const char *name, const std::string &val ) const;
};

std::ostream&	operator<<( std::ostream &o, const HttpResponse &res );

#endif
