/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 06:20:04 by joshua            #+#    #+#             */
/*   Updated: 2024/10/08 03:16:43 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP
# include "Http.hpp"

class HttpResponse : Http {
	public:
		HttpResponse( void );
		HttpResponse( bool proxy_ );
		~HttpResponse( void );
		void	setHeader( int status, const std::string & = std::string() );
		void	setContent( const std::string &type, uint64_t len );
		void	addBody( const std::string &str );

		size_t	getBodyLength( void ) const; // empty

	private:
		int			status;
		std::string	header;
		std::string	body;
		bool		proxy;
		std::string	final;
		size_t		bytes_sent;

		void	addField( const char *name, const std::string &val );
		void	addField( const char *name, const char *val );
		void	addField( const std::string &name, const std::string &val );
		void	addField( std::string &str, const char *name, const std::string &val ) const;
};

#endif
