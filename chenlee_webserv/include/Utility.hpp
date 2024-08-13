/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 22:42:52 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/10 12:25:43 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <map>

typedef std::map<int, std::string> IntStringMap;

namespace utl
{
	/**
	 * Splits a given string into a vector of substrings based on delimiter (either space or newline).
	 *
	 * @param str The string to be split
	 * @param delim The delimiter
	 * @return A vector of substrings extracted from the input string.
	 */
	std::vector<std::string> splitStringByDelim(const std::string &str, const char delim);

	bool isDirectory(const std::string &path);

	/**
	 * Converts a value of any type that supports the output stream operator (operator<<)
	 * to a std::string. This function template leverages std::ostringstream to perform
	 * the conversion, ensuring type safety and automatic handling of different data types.
	 * @param value The value of type T to be converted into a string.
	 * @return The string representation of the input value.
	 */
	template <typename T>
	std::string toString(T value)
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}
}

#endif
