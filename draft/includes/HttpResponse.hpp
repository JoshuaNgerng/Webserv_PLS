/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 06:20:04 by joshua            #+#    #+#             */
/*   Updated: 2024/09/28 23:25:19 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP
# include "Http.hpp"

class HttpResponse {
	public:
		HttpResponse( void );
		HttpResponse( bool proxy_ );
		~HttpResponse( void );
		void	getHeader( const std::string &str );
		void	getBody( const std::string &str );
		void	setStatus( int status );

	private:
		int			status;
		std::string	header;
		std::string	body;
		bool		proxy;
		std::string	final;
		size_t		bytes_sent;
};

#endif
