/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinistr <ksinistr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/22 14:48:23 by ksinistr          #+#    #+#             */
/*   Updated: 2020/09/08 10:35:25 by ksinistr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		cut_buf(char *buf, size_t buf_size, size_t n_index)
{
	buf_size = buf_size - n_index;
	ft_memmove(buf, &buf[n_index + 1], buf_size);
	buf[buf_size] = '\0';
	return (1);
}

int		fill_buf(int fd, char *buf, size_t *buf_size)
{
	int	buf_readed;

	buf_readed = read(fd, buf, BUFFER_SIZE);
	if (buf_readed <= 0)
		return (buf_readed);
	buf[buf_readed] = '\0';
	*buf_size = ft_strlen(buf);
	return (buf_readed);
}

int		fill_line(size_t *n_index, char *buf, size_t buf_size, char **line)
{
	size_t	new_line_size;
	char	*tmp;
	int		line_len;

	tmp = NULL;
	*n_index = 0;
	line_len = ft_strlen(*line);
	while (buf[*n_index] != '\n' && *n_index < buf_size)
		(*n_index)++;
	new_line_size = line_len + *n_index + 1;
	if (!(tmp = (char *)ft_calloc(new_line_size, sizeof(char))))
		return (-1);
	ft_memcpy(tmp, *line, line_len);
	ft_memcpy(&tmp[line_len], buf, *n_index);
	free(*line);
	*line = tmp;
	return (1);
}

int		read_line(int fd, char **line, char *buf, size_t buf_size)
{
	static int		buf_readed;
	size_t			n_index;

	n_index = 0;
	while (n_index == buf_size)
	{
		buf_size = ft_strlen(buf);
		if (n_index == buf_size || buf_readed <= 0)
		{
			buf_readed = fill_buf(fd, buf, &buf_size);
			if (buf_readed <= 0)
				return (buf_readed);
		}
		if (fill_line(&n_index, buf, buf_size, line) == -1)
			return (-1);
		if (buf_readed < BUFFER_SIZE && n_index == buf_size)
		{
			buf_readed = 0;
			return (0);
		}
	}
	return (cut_buf(buf, buf_size, n_index));
}

int		get_next_line(int fd, char **line)
{
	static char		*buf = NULL;
	int				ret;

	if (BUFFER_SIZE == 0 || line == NULL)
		return (-1);
	if (buf == NULL)
	{
		if (!(buf = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char))))
			return (-1);
		buf[0] = '\0';
	}
	if (!(*line = (char *)malloc(sizeof(char))))
		return (-1);
	(*line)[0] = '\0';
	ret = read_line(fd, line, buf, 0);
	if (ret <= 0)
	{
		free(buf);
		buf = NULL;
	}
	return (ret);
}
