/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftorn <ftorn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:40:21 by ftorn             #+#    #+#             */
/*   Updated: 2020/05/20 15:08:36 by ftorn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	counter;
	size_t	index;
	char	*res;

	counter = 0;
	index = 0;
	if (*little == '\0')
		return ((char*)big);
	while (counter < len && big[counter] != '\0')
	{
		res = (char*)&big[counter];
		while (big[counter + index] == little[index] && counter + index < len
				&& little[index] != '\0' && big[counter + index] != '\0')
			index++;
		if (little[index] != '\0')
			index = 0;
		else
			return ((char*)res);
		counter++;
	}
	return (NULL);
}
