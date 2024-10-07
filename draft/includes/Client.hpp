/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 09:20:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/07 17:57:20 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "ServerInfo.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

class Client {
	typedef std::list<Client>::const_iterator		client_ptr;
	typedef std::vector<ServerInfo>::const_iterator	server_ptr;
	typedef std::vector<Location>::const_iterator	loc_ptr;
	public:
		Client( void );
		Client( server_ptr &it );
		Client( const Client &src );
		~Client( void );

		Client&	operator=( const Client &src );
		Client&	operator=( const std::vector<Location>::const_iterator &it );

		int					clientSocketFd( int fd );
		bool				clientRecv( void );
		bool				checkReponse( void );
		void				routeRequest( void );
		void				addContent( int status_code,
										const std::string &str = std::string(),
										size_t	content_length = 0 );
		void				addDir( const std::string &str );

		/* getters */
		int					clientSocketFd( void ) const;
		int					getContentFd( void ) const;
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
		/* servtrer related info + data fd*/
		server_ptr	server_ref;
		loc_ptr		location_ref;

		sockaddr_storage_t	client_addr;
		socklen_t			len;
		int					socket_fd;
		int					content_fd;
		std::string			content_name;
		bool				fetch_content;
		uint64_t			length;
		int					status_code;
		bool				is_directory;
		bool				is_cgi;
		bool				finish_response;

		/* http related info + data info */
		std::queue<HttpRequest>	requests;
		HttpResponse			reponse;
		time_t					start_connection;
		size_t					no_request;
		time_t					current_time;
		time_t					empty_event;
		size_t					bytes_sent;
		bool					emergency_overwrite;
		bool					completed;

		void	processReponseSucess( void );
		void	processReponseRedirect( void );
		void	processReponseError( void );
		bool	fetchContentFd( void );
};

std::ostream&	operator<<( std::ostream &o, const Client &ref );

#endif
