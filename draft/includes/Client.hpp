/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:20:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/07 01:06:56 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "ServerInfo.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

class Client {
	typedef std::list<Client>::iterator	client_ptr;
	public:
		Client( void );
		Client( std::vector<ServerInfo>::iterator &it );
		Client( const Client &src );
		~Client( void );

		Client&	operator=( const Client &src );
		Client&	operator=( const std::vector<Location>::const_iterator &it );

		int					clientSocketFd( int fd );
		int					findResource( void );
		bool				clientRecv( void );
		bool				checkReponse( void );
		void				routeRequest( void );
		void				addResource( int status_code,
										const std::string &str = std::string());
		void				addDir( const std::string &str );

		/* getters */
		int					clientSocketFd( void ) const;
		int					getResourceFd( void ) const;
		const std::string&	getCurrentUri( void ) const;
		bool				checkHttpResponse( void ) const;
		bool				requestReady( void ) const;
		// bool				checkRequest( void ) const;
		// bool				checkResponse( void ) const;
		// const std::string&	getRequest( void ) const;
		// const std::string&	getResponse( void ) const;
		// size_t				getBytesSent( void ) const;


		std::vector<ServerInfo>::const_iterator	getServerRef( void ) const;
		std::vector<Location>::const_iterator	getLocationRef( void ) const;
		std::map<int, client_ptr>::iterator	getSocketRef( void ) const;
		std::map<int, client_ptr>::iterator	getReponseRef( void ) const;

	private:
		static const int	recv_flag = 0;
		static const int	send_flag = 0;
		static const size_t	recv_buffer_size = 8192;
		/* server related info + data fd*/
		std::vector<ServerInfo>::const_iterator	server_ref;
		std::vector<Location>::const_iterator	location_ref;

		sockaddr_storage_t	client_addr;
		socklen_t			len;
		int					socket_fd;
		int					resource_fd;
		std::string			resource_name;
		bool				have_resource;
		int					status_code;

		/* http related info + data info */
		std::queue<HttpRequest>				requests;
		HttpResponse						reponse;
		time_t								start_connection;
		size_t								no_request;
		time_t								current_time;
		time_t								empty_event;
		size_t								bytes_sent;
		bool								emergency_overwrite;
		bool								is_directory;
		bool								is_cgi;
		bool								finish_response;
};

std::ostream&	operator<<( std::ostream &o, const Client &ref );

#endif
