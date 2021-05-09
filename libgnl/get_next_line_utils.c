/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: resther <resther@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/22 14:48:11 by ksinistr          #+#    #+#             */
/*   Updated: 2021/05/09 17:03:09 by resther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_memcpy2(void *dst, const void *src, size_t n)
{
	size_t		i;

	if (dst == NULL && src == NULL)
		return (dst);
	i = -1;
	while (++i < n)
	{
		((char *)dst)[i] = ((const char *)src)[i];
	}
	return (dst);
}

size_t	ft_strlen2(const char *s)
{
	size_t	i;

	i = 0;
	while (*s)
	{
		i++;
		s++;
	}
	return (i);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t		i;
	char		*d;
	const char	*s;

	if (len == 0 || (dst == NULL && src == NULL))
		return (dst);
	d = dst;
	s = src;
	if (dst > src)
	{
		i = len - 1;
		while (i > 0)
		{
			d[i] = s[i];
			i--;
		}
		d[0] = s[0];
	}
	else
	{
		i = -1;
		while (++i < len)
			d[i] = s[i];
	}
	return (dst);
}

void	*ft_calloc2(size_t count, size_t size)
{
	void	*ret;
	size_t	i;

	ret = (void *)malloc(count * size);
	if (!ret)
		return (NULL);
	i = -1;
	while (++i < count * size)
		((char *)ret)[i] = '\0';
	return (ret);
}
