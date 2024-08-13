/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:45:42 by jatan             #+#    #+#             */
/*   Updated: 2024/08/10 19:31:59 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <iostream>
// #include <chrono>
#include <ctime>

#include "colors.h"

class Logger
{
public:
	Logger(std::string context);
	Logger(const Logger &src);
	Logger &operator=(const Logger &src);
	~Logger();

	void log(std::string str) const;
	void info(std::string str) const;
	void warning(std::string str) const;
	void error(std::string str) const;

private:
	void output(std::string color, std::string str) const;
	Logger();
	std::string _context;
};

std::ostream &			operator<<( std::ostream & o, Logger const & i );

#endif
