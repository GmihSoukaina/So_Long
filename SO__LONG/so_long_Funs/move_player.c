/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 17:27:39 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/09 17:59:44 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	player_moves(int i, int j, t_game *game)
{
	static size_t	move;
	char			p;

	count_collected(game);
	p = game->map[game->player_y + i][game->player_x + j];
	if (p == 'C')
		game->collectibles--;
	if (p != '1')
	{
		move++;
		game->moves = move;
		ft_putnbr(move);
		write(1, "\n", 1);
		if (game->player_x == game->exit_x && game->player_y == game->exit_y)
			game->map[game->player_y][game->player_x] = 'E';
		else
			game->map[game->player_y][game->player_x] = '0';
		game->map[game->player_y + i][game->player_x + j] = 'P';
		game->player_y += i;
		game->player_x += j;
		draw_map(game);
		if (p == 'E' && !game->collectibles)
			(write(1, "ela slametak nadi ^_^ \n", 28), mlx_map_destroyer(game));
	}
}

int	key_press(int keycode, t_game *game)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (keycode == 53)
		mlx_map_destroyer(game);
	if (keycode == 124)
		j = 1;
	else if (keycode == 123)
		j = -1;
	else if (keycode == 125)
		i = 1;
	else if (keycode == 126)
		i = -1;
	if (i != 0 || j != 0)
		player_moves(i, j, game);
	return (1);
}

void	draw_map(t_game *game)
{
	mlx_clear_window(game->mlxs.mlx, game->mlxs.mlx_win);
	game->i = 0;
	while (game->map[game->i])
	{
		game->j = 0;
		while (game->map[game->i][game->j])
		{
			if (game->map[game->i][game->j] == 'E')
				mlx_put_image_to_window(game->mlxs.mlx, game->mlxs.mlx_win,
					game->exit, game->j * 40, game->i * 40);
			else if (game->map[game->i][game->j] == 'P')
				mlx_put_image_to_window(game->mlxs.mlx, game->mlxs.mlx_win,
					game->player, game->j * 40, game->i * 40);
			else if (game->map[game->i][game->j] == '1')
				mlx_put_image_to_window(game->mlxs.mlx, game->mlxs.mlx_win,
					game->wall, game->j * 40, game->i * 40);
			else if (game->map[game->i][game->j] == 'C')
				mlx_put_image_to_window(game->mlxs.mlx, game->mlxs.mlx_win,
					game->coin, game->j * 40, game->i * 40);
			game->j++;
		}
		game->i++;
	}
}

void	xpm_to_img(t_game *game)
{
	game->player = mlx_xpm_file_to_image(game->mlxs.mlx, "images/player.xpm",
			&game->img_width, &game->img_height);
	game->exit = mlx_xpm_file_to_image(game->mlxs.mlx, "images/exit.xpm",
			&game->img_width, &game->img_height);
	game->wall = mlx_xpm_file_to_image(game->mlxs.mlx, "images/wall.xpm",
			&game->img_width, &game->img_height);
	game->coin = mlx_xpm_file_to_image(game->mlxs.mlx, "images/collectible.xpm",
			&game->img_width, &game->img_height);
	if (!game->player || !game->exit || !game->wall || !game->coin)
	{
		write(2, "Error\nimage name is not compatible!\n", 35);
		mlx_map_destroyer(game);
	}
}

void	render_map(t_game *game)
{
	game->moves = 0;
	game->mlxs.mlx = mlx_init();
	game->mlxs.mlx_win = mlx_new_window(game->mlxs.mlx, 1200, 1200, "so_long");
	xpm_to_img(game);
	draw_map(game);
	mlx_hook(game->mlxs.mlx_win, 2, 1L << 0, key_press, game);
	mlx_hook(game->mlxs.mlx_win, 17, 0, close_game, game);
	mlx_loop(game->mlxs.mlx);
}
