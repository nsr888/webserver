/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinistr <ksinistr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 23:12:40 by ksinistr          #+#    #+#             */
/*   Updated: 2020/05/11 16:41:19 by ksinistr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_size(int n, int sign)
{
	int	len;

	len = 0;
	if (n == 0)
		len = 1;
	while (n > 0)
	{
		n = n / 10;
		len++;
	}
	len = len + sign;
	return (len);
}

static void	fill_ret(char *ret, int n, int len, int sign)
{
	while (n > 9)
	{
		ret[len] = (char)(n % 10) + '0';
		n = n / 10;
		len--;
	}
	ret[len] = (char)n + '0';
	if (sign)
		ret[--len] = '-';
}

static char	*ft_itoa2(int n, char *ret)
{
	int		len;
	int		sign;

	sign = 0;
	if (n == -2147483648)
		return ("-2147483648");
	if (n < 0)
	{
		sign = 1;
		n = -1 * n;
	}
	len = count_size(n, sign);
	ft_bzero(ret, 12);
	len--;
	fill_ret(ret, n, len, sign);
	return (ret);
}

void		ft_putnbr_fd(int n, int fd)
{
	char	ret[12];

	ft_putstr_fd(ft_itoa2(n, ret), fd);
}
