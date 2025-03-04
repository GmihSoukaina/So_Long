/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funs_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 12:20:20 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/04 12:26:07 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	print_error(char *str)
{
	write (2, "Error\n", 6);
	write(2, str, ft_strlen(str));
	exit(1);
}

int	count_lines(t_game *game)
{
	int		lines;
	char	*line;
	int		fd;

	lines = 0;
	fd = game->fd;
	line = get_next_line(fd);
	while (line)
	{
		lines++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (lines);
}

void	free_map(char **map, int i)
{
	while (i >= 0)
	{
		free(map[i]);
		i--;
	}
	free(map);
}

void	free_game(t_game *game)
{
	if (game->map)
		free_map(game->map, game->i);
	if (game->map2)
		free_map(game->map2, game->i);
	if (game)
		free(game);
}

void	empty_line(t_game *game, char *line)
{
	if (!line)
	{
		print_error("map is empty\n");
		close(game->fd);
		free_map(game->map, game->i);
		return ;
	}
}
