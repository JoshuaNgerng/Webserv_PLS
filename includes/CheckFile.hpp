/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckFile.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:45:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/17 15:12:27 by jngerng          ###   ########.fr       */
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
	open = 1,
	read = 2
};

class CheckFile {
	public:
		CheckFile( const char *filename_ );
		~CheckFile( void );

		void	checking( void );
		void	checking( int check_flags );
		uint8_t	getType( void ) const;
		uint8_t	getAccessbility( void ) const;
		uint8_t	getFileContent( std::string &dst ) const;
		uint8_t	getFileContent( std::stringstream &dst ) const;

		static bool	fileToStringStream( std::stringstream &dst, std::ifstream &file );

	private:
		CheckFile( void );

		std::string	filename;
		uint8_t		type;
		int8_t		acessiblity;
};

#endif
