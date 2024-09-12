/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckFile.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:45:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/12 12:57:15 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKFILE_HPP
# define CHECKFILE_HPP
# include "Const.hpp"

enum filetype {
	block_device = 1,
	char_device = 2,
	directory = 3,
	fifo_pipe = 4,
	symlink = 5,
	file = 6,
	sock = 7,
	unknown = 8,
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
		// status change st_ctime
		// last access st_atime
		// last modifi st_mtime
};

#endif
