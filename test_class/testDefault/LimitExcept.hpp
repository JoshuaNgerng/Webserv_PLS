/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExcept.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 16:46:13 by joshua            #+#    #+#             */
/*   Updated: 2024/10/22 23:32:14 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIMIT_HPP
# define LIMIT_HPP
# include "Http.hpp"

class LimitExcept {
	public:
		LimitExcept( void );
		LimitExcept( const LimitExcept &src );
		~LimitExcept( void );
		void	addMethods( const std::string& methods );
		void	addAllow( const std::string& str );
		void	addDeny( const std::string& str );
		bool	checkAccess( int method, const std::string &client );

	private:
		std::vector<int>			methods;
		std::vector<std::string>	allow;
		std::vector<std::string>	deny;
};

#endif
