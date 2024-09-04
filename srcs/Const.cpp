/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Const.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:01:33 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/04 15:03:20 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Const.hpp"

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
