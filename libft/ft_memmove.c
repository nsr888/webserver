/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinistr <ksinistr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 16:07:02 by ksinistr          #+#    #+#             */
/*   Updated: 2020/05/06 19:52:15 by ksinistr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
