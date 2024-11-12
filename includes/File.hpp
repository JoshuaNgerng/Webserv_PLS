/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:12:03 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/12 12:47:34 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
# define FILE_HPP
# include "Const.hpp"

class File {
	public:
		int					getInputFd( void ) const;
		int					getOutputFd( void ) const;
		int					getFdCounter( void ) const;
		const std::string&	getContentId( void ) const;

		void				setContentPath( const std::string &root, const std::string &fname );
		void				setContentPath( const std::string &path );
		void				serverReceivedFds( void );
		bool				processFds( void );
		bool				getFds( void ) const;
		void				getFds( int fds[2] ) const;

		virtual bool	generateFds( void ) = 0;
		virtual bool	checkStatus( void ) = 0;

		virtual std::ostream&	print( std::ostream &o ) const = 0;

		virtual ~File( void );

	protected:
		std::string	content_id;
		bool		already_retrieved_fd;
		int			content_input_fd;
		int			content_output_fd;
		int			counter;

		File( void );
		File( const File &src );
		File&	operator=( const File &src );


	private:
		bool		processFd( int &fd ) const;
};

std::ostream&	operator<<( std::ostream &o, const File &ref );

#endif
