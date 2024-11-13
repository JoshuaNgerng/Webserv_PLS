/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckFile.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:45:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/13 21:28:39 by jngerng          ###   ########.fr       */
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
	systemlink = 5,
	file = 6,
	sock = 7,
	unknown = 8,
	error = UCHAR_MAX
};

class CheckFile {
	public:
		CheckFile( const char *filename_ );
		CheckFile( const std::string &filename_ );
		CheckFile( const CheckFile &src );
		CheckFile& operator=( const CheckFile &src );
		~CheckFile( void );

		void	checking( void );
		void	checking( int check_flags );
		uint8_t	getType( void ) const;
		int8_t	getAccessbility( void ) const;
		size_t	getFilesize( void ) const;
		
		const struct tm*	getTime( void ) const;
	
		bool		getFileContent( std::string &dst ) const;
		bool		getFileContent( std::stringstream &dst ) const;
		const char	*fetchExtension( void ) const;

		static bool			checkAccessbility( const char *path, int mask );
		static const char	*fetchExtension( const std::string &fname );
		static bool	fileToStringStream( std::stringstream &dst, std::ifstream &file );

	private:
		CheckFile( void );

		const char *filename;
		uint8_t		type;
		int8_t		acessiblity;
		size_t		filesize;
		struct tm*	timeinfo;
		// status change st_ctime
		// last access st_atime
		// last modifi st_mtime
};

#endif
