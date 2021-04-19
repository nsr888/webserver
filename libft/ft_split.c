/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinistr <ksinistr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/19 12:29:42 by ksinistr          #+#    #+#             */
/*   Updated: 2020/05/20 09:37:43 by ksinistr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	i;

	i = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s == '\0')
			break ;
		if (*s != c)
		{
			i++;
			while (*s != c && *s)
				s++;
		}
	}
	return (i);
}

static void	*clear_array(char **ret, int i)
{
	int	j;

	j = -1;
	while (++j < i)
	{
		free(ret[i]);
		ret[i] = NULL;
	}
	return (NULL);
}

static char	**fill_ret(char const *s, char c, char **ret, int i)
{
	int		len;
	char	*start;

	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s == '\0')
			break ;
		if (*s != c)
		{
			len = 0;
			start = (char *)s;
			while (*s != c && *s)
			{
				s++;
				len++;
			}
			if (!(ret[i] = (char *)malloc((len + 1) * sizeof(char))))
				return (clear_array(ret, i));
			ft_strlcpy(ret[i], (const char *)start, len + 1);
			i++;
		}
	}
	return (ret);
}

char		**ft_split(char const *s, char c)
{
	char	**ret;
	int		size;

	if (!s)
		return (NULL);
	size = count_words(s, c);
	if (!(ret = (char **)malloc((size + 1) * sizeof(char *))))
		return (NULL);
	ret[size] = NULL;
	ret = fill_ret(s, c, ret, 0);
	return (ret);
}
