/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 12:30:29 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/07 16:15:02 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <stdio.h>
# include <mlx.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "../lib/libft/libft.h"
# include "../lib/gnl/get_next_line.h"

typedef struct s_mlx
{
	void	*mlx;
	void	*mlx_win;
}	t_mlx;

typedef struct s_game
{
	t_mlx	mlxs;
	int		width;
	int		height;
	void	*img;
	int		i;
	int		j;
	int		fd;
	char	**map;
	char	**map2;
	int		map_width;
	int		map_height;
	int		img_width;
	int		img_height;
	int		collectibles;
	int		player_x;
	int		player_y;
	int		exit_x;
	int		exit_y;
	void	*player;
	void	*floor;
	void	*wall;
	int		moves;
}	t_game;

void	read_map(char *filename, t_game *game);
void	check_map(t_game *game, char *line);
void	valid_map(t_game *game, char *line);
int		is_closed(t_game *game);
int		check_walls(t_game *game);
void	empty_line(t_game *game, char *line);
void	free_map(char **map, int height);
void	free_game(t_game *game);
void	print_error(char *error);
int		count_lines(t_game *game);
int		is_open(char *filename, t_game *game);
void	flood_fill(t_game *game, int y, int x);
void	check_collectibles(t_game *game);
int	is_valid_rules(t_game *game);
void mlx_map_destroyer(t_game *game);
int	close_game(t_game *game);

#endif