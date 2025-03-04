/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 12:01:47 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/04 12:06:37 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int check_walls(t_game *game)
{
	game->i = 0;
	game->j = 0;
	while (game->map[game->i][game->j])
	{
		if (game->map[game->i][game->j++] != '1')
			return (0);
	}
	game->i = game->map_height - 1;
	game->j = 0;
	while (game->map[game->i][game->j])
	{
		if (game->map[game->i][game->j++] != '1')
			return (0);
	}
	game->i = 1;
	game->j = game->map_width - 1;
	while (game->i < game->map_height)
	{
		if (game->map[game->i][0] != '1' || game->map[game->i][game->j] != '1')
			return (0);
		game->i++;
	}
	if (game->map_height != game->i)
		return (0);
	return (1);
}

int is_closed(t_game *game)
{
	game->map_height = 0;
	while (game->map[game->map_height])
		game->map_height++;
	game->map_width = ft_strlen(game->map[0]);
	if (!check_walls(game))
	{
		print_error("map is not closed\n");
		free_map(game->map, game->map_height);
		free_map(game->map2, game->map_height);
		exit(1);
	}
	return (1);
}

void	valid_map(t_game *game, char *line)
{
	size_t	len;

	if ((ft_strcmp(line, "\n") == 0) || ft_strlen(line) == 0)
	{
		print_error("map is invalid\n");
		free(line);
		close(game->fd);
		free_map(game->map, game->i);
		return ;
	}
	len = ft_strlen(game->map[game->i]);
	if (len > 0 && game->map[game->i][len - 1] == '\n')
		game->map[game->i][len - 1] = '\0';
	if (game->i > 0
		&& ft_strlen(game->map[game->i]) != ft_strlen(game->map[game->i - 1]))
	{
		print_error("map is not a rectangle\n");
		free(line);
		close(game->fd);
		free_map(game->map, game->i);
		return ;
	}
}

void	check_map(t_game *game, char *line)
{
	char	*trimmed_line;

	trimmed_line = ft_strtrim(line, "\n");
	game->map[game->i] = ft_strdup(trimmed_line);
	game->map2[game->i] = ft_strdup(trimmed_line);
	free(trimmed_line);
	if (!game->map[game->i] || !game->map2[game->i])
	{
		print_error("Memory allocation failed for line\n");
		free(line);
		close(game->fd);
		free_map(game->map, game->i);
		free_map(game->map2, game->i);
		return ;
	}
	valid_map(game, line);
}

