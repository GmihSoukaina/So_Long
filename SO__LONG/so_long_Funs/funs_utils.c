/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funs_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 17:42:12 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/09 17:49:06 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	player_position(t_game *game)
{
	game->i = 0;
	while (game->map[game->i])
	{
		game->j = 0;
		while (game->map[game->i][game->j])
		{
			if (game->map[game->i][game->j] == 'P')
			{
				game->player_x = game->j;
				game->player_y = game->i;
			}
			game->j++;
		}
		game->i++;
	}
}

void	exit_position(t_game *game)
{
	game->i = 0;
	while (game->map[game->i])
	{
		game->j = 0;
		while (game->map[game->i][game->j])
		{
			if (game->map[game->i][game->j] == 'E')
			{
				game->exit_x = game->j;
				game->exit_y = game->i;
				return ;
			}
			game->j++;
		}
		game->i++;
	}
}

void	count_collected(t_game	*game)
{
	game->i = 0;
	game->collectibles = 0;
	while (game->map[game->i])
	{
		game->j = 0;
		while (game->map[game->i][game->j])
		{
			if (game->map[game->i][game->j] == 'C')
				game->collectibles++;
			game->j++;
		}
		game->i++;
	}
}

void	mlx_map_destroyer(t_game *game)
{
	if (game->mlxs.mlx_win)
		mlx_destroy_window(game->mlxs.mlx, game->mlxs.mlx_win);
	if (game->img)
		mlx_destroy_image(game->mlxs.mlx, game->img);
	if (game->map)
		free_map(game->map, game->i);
	if (game->map2)
		free_map(game->map2, game->j);
	free(game);
	exit(0);
}

int	close_game(t_game *game)
{
	if (game)
	{
		if (game->map)
			free_map(game->map, game->i);
		if (game->map2)
			free_map(game->map2, game->j);
		free(game);
	}
	exit(0);
	return (0);
}
