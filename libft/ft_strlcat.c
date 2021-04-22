/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinistr <ksinistr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/04 14:18:19 by ksinistr          #+#    #+#             */
/*   Updated: 2020/05/19 12:09:29 by ksinistr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	result_len;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dst_len < dstsize)
		result_len = dst_len + src_len;
	else
		return (dstsize + src_len);
	if (src_len + dst_len < dstsize)
	{
		dst[dst_len + src_len] = '\0';
		ft_memcpy(&dst[dst_len], src, src_len);
	}
	else
	{
		dst[dstsize - 1] = '\0';
		ft_memcpy(&dst[dst_len], src, dstsize - dst_len - 1);
	}
	return (result_len);
}
