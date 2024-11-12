/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:52:32 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/05 18:09:58 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATICFILE_HPP
# define STATICFILE_HPP
# include "File.hpp"

class StaticFile : public File {
	public:
		StaticFile( void );
		StaticFile( const StaticFile &src );
		~StaticFile( void );
		StaticFile&	operator=( const StaticFile &src );
		bool	generateFds( void );
		bool	checkStatus( void );

		std::ostream&	print( std::ostream& o ) const;
};

std::ostream&	operator<<( std::ostream& o, const StaticFile &ref );

#endif
