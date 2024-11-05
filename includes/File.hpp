/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:12:03 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/05 11:03:07 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
# include "Const.hpp"

class File {
	public:
		File( void );
		File( const File &src );
		~File( void );
		File&	operator=( const File &src );

		void			setContentPath( const std::string &root, const std::string &fname);
		bool			getFd( void ) const;
		virtual bool	makeFd( void );
		virtual void	getFd( int fds[2] ) const;
	private:
		int			content_fd;
		std::string	content_path;
		bool		already_retrieved_fd;
};

#endif
