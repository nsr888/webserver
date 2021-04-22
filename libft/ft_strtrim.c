/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinistr <ksinistr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/07 10:13:13 by ksinistr          #+#    #+#             */
/*   Updated: 2020/05/12 15:55:50 by ksinistr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*fill(char *ret, const char *s1, int cnt)
{
	int	size;

	size = ft_strlen(s1) + 1 - cnt;
	ret = (char *)malloc(size * sizeof(char));
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, s1, size);
	return (ret);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	char		*ret;
	const char	*tmp;
	int			cnt;

	ret = NULL;
	if (!s1)
		return (NULL);
	while (*s1)
	{
		if (!ft_strchr(set, *s1))
			break ;
		s1++;
	}
	if (*s1 == '\0')
		return (ft_strdup(s1));
	tmp = &s1[ft_strlen(s1) - 1];
	cnt = 0;
	while (*tmp)
	{
		if (!ft_strchr(set, *tmp))
			break ;
		cnt++;
		tmp--;
	}
	return (fill(ret, s1, cnt));
}
