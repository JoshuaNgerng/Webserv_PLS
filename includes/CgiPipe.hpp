/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiPipe.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:49:32 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/07 00:39:55 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIPIPE_HPP
# define CGIPIPE_HPP
# include "Const.hpp"
# include "File.hpp"

class CgiPipe : public File {
	public:
		CgiPipe( void );
		CgiPipe( const CgiPipe &src );
		~CgiPipe( void );
		CgiPipe&	operator=( const CgiPipe &src );

		bool	generateFds( void );
		bool	checkStatus( void );

		std::ostream&	print( std::ostream &o ) const;

	private:
		int			status;
		bool		kill_child;
		pid_t		child_id;
		std::string	bin_path;

		bool	setupPipes( int pipefd[4] );
		int		execvChild( int pipefd[4] );
};

#endif