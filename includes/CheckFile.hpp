/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckFile.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:45:59 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/08 19:02:27 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Const.hpp"

enum filetype {
	file = 1,
	folder = 2,
	unknown = 4,
	error = UCHAR_MAX
};

class CheckFile {
	public:
		CheckFile( const char *filename_ );
		~CheckFile( void );

		void	checking( void );
		void	checking( int check_flags );
		uint8_t	getType( void ) const;
		uint8_t	getAccessbility( void ) const;
		std::string	getFileContent( void ) const;

	private:
		CheckFile( void );
		std::string	filename;
		uint8_t		type;
		int8_t		acessiblity;
};
