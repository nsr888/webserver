/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: resther <resther@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/22 14:47:28 by ksinistr          #+#    #+#             */
/*   Updated: 2021/05/09 17:04:39 by resther          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUFFER_SIZE 10
# include <unistd.h>
# include <stdlib.h>

int		get_next_line(int fd, char **line);
size_t	ft_strlen2(const char *s);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	*ft_memcpy2(void *dst, const void *src, size_t n);
void	*ft_calloc2(size_t count, size_t size);

#endif
