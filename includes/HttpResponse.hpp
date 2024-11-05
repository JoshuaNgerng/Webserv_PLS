/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 06:20:04 by joshua            #+#    #+#             */
/*   Updated: 2024/11/05 18:13:14 by jngerng          ###   ########.fr       */
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

		void	addBody( const std::string &str );
		void	addBody( const char *str, size_t bytes );
		void	setHeader( int status, const std::string & = std::string() );
		void	setContent( const std::string &type, uint64_t len );
		void	setContent( void );
		void	reset( void );

		size_t	getBodyLength( void ) const;
		size_t	getTotalLength( void ) const;
		bool	isReady( void ) const;

	private:
		int		status;
		bool	proxy;

		void	addField( const char *name, const std::string &val );
		void	addField( const char *name, const char *val );
		void	addField( const std::string &name, const std::string &val );
		void	addField( std::string &str, const char *name, const std::string &val ) const;
};

std::ostream&	operator<<( std::ostream &o, const HttpResponse &res );

#endif
