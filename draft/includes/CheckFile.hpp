/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckFile.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:45:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/02 04:20:26 by jngerng          ###   ########.fr       */
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
		~CheckFile( void );

		void	checking( void );
		void	checking( int check_flags );
		void	checkExtension( void );
		uint8_t	getType( void ) const;
		uint8_t	getAccessbility( void ) const;
		size_t	getFilesize( void ) const;
		
		const struct tm*	getTime( void ) const;
	
		bool		getFileContent( std::string &dst ) const;
		bool		getFileContent( std::stringstream &dst ) const;
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
