/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiPipe.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:49:32 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/12 21:04:36 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIPIPE_HPP
# define CGIPIPE_HPP
# include "Const.hpp"
# include "File.hpp"

class CgiPipe : public File {
	public:
		CgiPipe( void );
		CgiPipe( const std::string &bin, Client &client );
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
				CgiPipe	*ptr;
			public:
				ChildProcess( CgiPipe &ref );
				~ChildProcess( void ) throw();
				virtual const char *what() const throw();
				int	execveCgiPipe( void );
		};

	private:
		int							status;
		bool						kill_child;
		pid_t						child_id;
		std::string					directory;
		std::string					bin_path;
		std::vector<std::string>	req_info;
		std::vector<const char*>	env;
		int							pipefd[4];
		Client 						*ptr;
		bool						child_done;

		bool	changeDir( void );
		bool	getBinary( void );
		bool	setupPipes( void );
		int		execvChild( void );
		void	getClientInfo( const Client &client );
		void	getClientInfoHelper( const char * key, const std::string &val );
		void	getClientInfoReq( const HttpRequest &req );
};

#endif
