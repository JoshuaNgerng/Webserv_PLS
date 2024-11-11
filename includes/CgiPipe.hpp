/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiPipe.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:49:32 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/10 23:48:47 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIPIPE_HPP
# define CGIPIPE_HPP
# include "Const.hpp"
# include "File.hpp"

class CgiPipe : public File {
	public:
		CgiPipe( void );
		CgiPipe( const std::string &bin, const Client &client );
		CgiPipe( const CgiPipe &src );
		~CgiPipe( void );
		CgiPipe&	operator=( const CgiPipe &src );

		bool	generateFds( void );
		bool	checkStatus( void );
		void	addEnv( const Client &client, char *const *env );
		void	addDir( const std::string &path );
		void	addBin( const std::string &content_name, const std::string &bin );

		std::ostream&	print( std::ostream &o ) const;
		class ChildProcess : std::exception {
			private:
				int	status_code;
			public:
				ChildProcess( int status );
				~ChildProcess( void ) throw();
				virtual const char *what() const throw();
				int	getExitCode( void ) const;
		};

	private:
		int							status;
		bool						kill_child;
		pid_t						child_id;
		std::string					directory;
		std::string					bin_path;
		std::vector<std::string>	req_info;
		std::vector<const char*>	env;
		const Client 				*ptr;

		bool	changeDir( void );
		bool	getBinary( void );
		bool	setupPipes( int pipefd[4] );
		int		execvChild( int pipefd[4] );
		void	getClientInfo( const Client &client );
		void	getClientInfoHelper( const char * key, const std::string &val );
		void	getClientInfoReq( const HttpRequest &req );
};

#endif