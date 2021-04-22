/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinistr <ksinistr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/07 09:38:25 by ksinistr          #+#    #+#             */
/*   Updated: 2020/05/07 10:00:31 by ksinistr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ret;
	int		substr_size;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (ft_strlen(&s[start]) < len)
		substr_size = ft_strlen(&s[start]);
	else
		substr_size = len;
	ret = (char *)malloc((substr_size + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, &s[start], substr_size + 1);
	return (ret);
}
