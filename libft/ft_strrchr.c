/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinistr <ksinistr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 11:59:17 by ksinistr          #+#    #+#             */
/*   Updated: 2020/05/06 19:51:05 by ksinistr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*start;

	start = s;
	s = &s[ft_strlen(s)];
	if (*s == (unsigned char)c)
		return ((char *)s);
	s--;
	while (*s && s != start)
	{
		if (*s == (unsigned char)c)
			return ((char *)s);
		s--;
	}
	if (*s == (unsigned char)c)
		return ((char *)s);
	return (NULL);
}
