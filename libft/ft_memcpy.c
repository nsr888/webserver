/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinistr <ksinistr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 16:07:02 by ksinistr          #+#    #+#             */
/*   Updated: 2020/05/02 20:37:41 by ksinistr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t		i;
	char		*d;
	const char	*s;

	d = dst;
	s = src;
	if (dst == NULL && src == NULL)
		return (d);
	i = -1;
	while (++i < n)
	{
		d[i] = s[i];
	}
	return (d);
}
