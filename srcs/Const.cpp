/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Const.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:01:33 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/06 14:27:05 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Const.hpp"

static size_t ft_strlen(const char *s)
{
	size_t	len = -1;
	while (s[++ len]);
	return (len);
}

int	ft_strncpy(const char *s1, const char *s2, size_t n)
{
	while (*s1 == *s2 && *s1 != '\0' && *s2 != '\0' && n > 0)
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

size_t	ft_strstr(const char *haystack, const char *needle)
{
	size_t	i = -1;
	size_t	len; 
	if (!haystack)
		return (std::string::npos);
	if (!needle)
		return (0);
	len = ft_strlen(needle);
	if (!len)
		return (0);
	while (haystack[++ i])
	{
		if (haystack[i] == needle[0])
		{
			if (!ft_strncpy(&haystack[i], needle, len))
				return (i);
		}
	}
	return (std::string::npos);
}
