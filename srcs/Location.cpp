/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: folim <folim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 23:44:44 by folim             #+#    #+#             */
/*   Updated: 2024/09/06 23:44:46 by folim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Parse.hpp"

Location::Location(const std::string &path){
    std::cout << "Location contructor for path: " << path << "\n";
    this->path = path;
}
