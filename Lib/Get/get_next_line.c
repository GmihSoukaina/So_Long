/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:55:48 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/01 09:03:37 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*find_newline(int fd, char *str)
{
	char	*buffer;
	int		chars_read;
	char	*tmp;

	chars_read = 1;
	buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (!str)
		str = ft_strdup("");
	while (chars_read != 0 && (ft_strchr(str, '\n') == NULL))
	{
		chars_read = read(fd, buffer, BUFFER_SIZE);
		if (chars_read < 0)
			return (free(buffer), free(str), NULL);
		buffer[chars_read] = '\0';
		tmp = str;
		str = ft_strjoin(str, buffer);
		if (!str)
			return (free(buffer), free(tmp), NULL);
		free(tmp);
	}
	free(buffer);
	return (str);
}

static char	*substr_newline(char *str)
{
	size_t	len;
	char	*new_line;

	if (ft_strlen(str) == 0)
		return (NULL);
	len = 0;
	while (str[len] != '\n' && str[len] != '\0')
		len++;
	if (str[len] == '\n')
		len++;
	new_line = ft_substr(str, 0, len);
	if (!new_line)
		return (NULL);
	return (new_line);
}

static char	*update_str(char *str)
{
	int		i;
	int		j;
	char	*new_str;
	char	*new_pos;

	i = 0;
	new_pos = ft_strchr(str, '\n');
	if (new_pos)
		i = new_pos - str + 1;
	else
		i = ft_strlen(str);
	if (i == 0)
	{
		free(str);
		return (NULL);
	}
	new_str = (char *)malloc(sizeof(char) * (ft_strlen(str + i) + 1));
	if (!new_str)
		return (NULL);
	j = 0;
	while (str[i])
		new_str[j++] = str[i++];
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

char	*get_next_line(int fd)
{
	static char	*str;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	str = find_newline(fd, str);
	if (str == NULL)
		return (NULL);
	line = substr_newline(str);
	if (!line || *line == 0)
		return (free(str), str = NULL, NULL);
	str = update_str(str);
	return (line);
}
