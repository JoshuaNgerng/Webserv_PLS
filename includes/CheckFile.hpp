/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckFile.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:45:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/26 10:11:29 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKFILE_HPP
# define CHECKFILE_HPP
# include "Const.hpp"

enum filetype {
	file = 1,
	folder = 2,
	unknown = 4,
	error = UCHAR_MAX
};

enum checkfileerror {
	success = 0,
	open_ = 1,
	read_ = 2
};

class CheckFile {
	public:
		CheckFile( const char *filename_ );
		~CheckFile( void );

		void	checking( void );
		void	checking( int check_flags );
		uint8_t	getType( void ) const;
		uint8_t	getAccessbility( void ) const;
		size_t	getFilesize( void ) const;
		uint8_t	getFileContent( std::string &dst ) const;
		uint8_t	getFileContent( std::stringstream &dst ) const;

		static bool	fileToStringStream( std::stringstream &dst, std::ifstream &file );

	private:
		CheckFile( void );

		std::string	filename;
		uint8_t		type;
		int8_t		acessiblity;
		size_t		filesize;
};

#endif
