/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 06:20:04 by joshua            #+#    #+#             */
/*   Updated: 2024/10/22 23:13:00 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP
# include "Http.hpp"

class HttpResponse : Http {
	public:
		HttpResponse( void );
		HttpResponse( const HttpResponse &src );
		HttpResponse( bool proxy_ );
		~HttpResponse( void );
		HttpResponse&	operator=( const HttpResponse &src );

		void	setHeader( int status, const std::string & = std::string() );
		void	setContent( const std::string &type, uint64_t len );
		void	addBody( const std::string &str );
		void	finishResponseMsg( void );
		void	reset( void );

		size_t	getBodyLength( void ) const; // empty
		size_t	getTotalLength( void ) const; // empty
		const char *getPtrPos( size_t no_bytes_send ) const;

	private:
		int			status;
		std::string	header;
		std::string	body;
		bool		proxy;
		std::string	final;

		void	addField( const char *name, const std::string &val );
		void	addField( const char *name, const char *val );
		void	addField( const std::string &name, const std::string &val );
		void	addField( std::string &str, const char *name, const std::string &val ) const;
};

#endif
