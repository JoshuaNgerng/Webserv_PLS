/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 06:20:04 by joshua            #+#    #+#             */
/*   Updated: 2024/11/14 20:31:21 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP
# include "Http.hpp"

class HttpResponse : public Http {
	public:
		HttpResponse( void );
		HttpResponse( const HttpResponse &src );
		~HttpResponse( void );
		HttpResponse&	operator=( const HttpResponse &src );

		void	addFin( const std::string &str );
		void	addFin( const char *str, size_t bytes );
		void	addBody( const std::string &str );
		void	addBody( const char *str, size_t bytes );
		void	setHeader( int status, const std::string & = std::string() );
		void	setContent( uint64_t len, const std::string &type = std::string() );
		void	setContent( void );
		void	reset( void );

		bool	isReady( void ) const;
		bool	processCgiDataHeader( void );
		bool	processCgiData( void );

	private:
		int		status;

		bool	validateHttpStart( const std::string &line ) const;
		bool	generateHeader( const std::string &buffer );
		bool	generateHeaderHelper( const std::string &line );
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
