/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinistr <ksinistr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 09:16:52 by ksinistr          #+#    #+#             */
/*   Updated: 2020/05/06 19:54:42 by ksinistr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	overflow_value(int c)
{
	if (c == 1)
		return (-1);
	return (0);
}

static int	ft_isspace(int c)
{
	c = (unsigned char)c;
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' ||
			c == ' ')
		return (1);
	return (0);
}

int			ft_atoi(const char *str)
{
	int			sign;
	long int	n;
	long int	old_value;

	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	n = 0;
	old_value = 0;
	while (*str && ft_isdigit(*str))
	{
		old_value = n;
		n = (n * 10) + (*str - '0');
		if (n < old_value)
			return (overflow_value(sign));
		str++;
	}
	return ((int)(sign * n));
}
