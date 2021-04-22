/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinistr <ksinistr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/07 10:06:16 by ksinistr          #+#    #+#             */
/*   Updated: 2020/05/07 10:12:08 by ksinistr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ret;
	int		ret_len;

	if (!s1 || !s2)
		return (NULL);
	ret_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	ret = (char *)malloc(ret_len * sizeof(char));
	if (!ret)
		return (NULL);
	ret[0] = '\0';
	ft_strlcat(ret, s1, ret_len);
	ft_strlcat(ret, s2, ret_len);
	return (ret);
}
