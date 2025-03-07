/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:18:53 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/07 10:31:05 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	read_map(char *filename, t_game *game)
{
	char	*line;
	int		lines;

	lines = count_lines(game);
	game->fd = open(filename, O_RDONLY);
	game->map = malloc((lines + 1) * sizeof(char *));
	game->map2 = malloc((lines + 1) * sizeof(char *));
	if (!game->map || !game->map2)
	{
		print_error("Memory allocation failed\n");
		(free(game->map), free(game->map2), close(game->fd));
		return ;
	}
	game->i = 0;
	line = get_next_line(game->fd);
	empty_line(game, line);
	while (line)
	{
		(check_map(game, line), free(line));
		game->i++;
		line = get_next_line(game->fd);
	}
	game->map[game->i] = NULL;
	game->map2[game->i] = NULL;
	close(game->fd);
}

void	flood_fill(t_game *game, int y, int x)
{
    if (y < 0 || x < 0 || y >= game->map_height || x >= game->map_width)
        return;
    if (game->map2[y][x] == '1' || game->map2[y][x] == 'E' || game->map2[y][x] == 'X')
        return;
    if (game->map2[y][x] == 'C')
        game->collectibles--;
    game->map2[y][x] = 'X';
    flood_fill(game, y + 1, x);
    flood_fill(game, y - 1, x);
    flood_fill(game, y, x + 1);
    flood_fill(game, y, x - 1);
}

void	check_collectibles(t_game *game)
{
    game->i = 0;
    while (game->i < game->map_height)
    {
        game->j = 0;
        while (game->j < game->map_width)
        {
            if (game->map2[game->i][game->j] == 'C')
            {
                print_error("map is not valid.\n");
                exit(1);
            }
            // if (game->map2[game->i][game->j] == 'E')
            // {
            //     printf("Error: Exit at (%d, %d) is unreachable.\n", game->i, game->j);
            //     exit(1);
            // }
            game->j++;
        }
        game->i++;
    }
}

int	is_valid_rules(t_game *game)
{
	int		collec[256];

	ft_bzero(collec, 256);
	game->i = 1;
	while (game->map[game->i])
	{
		game->j = 1;
		while (game->map[game->i][game->j])
		{
			if (game->map[game->i][game->j] == '0' || game->map[game->i][game->j] == '1'
					|| game->map[game->i][game->j] == 'C' || game->map[game->i][game->j] == 'E'
					|| game->map[game->i][game->j] == 'P')
				collec[(int)game->map[game->i][game->j]] += 1;
			else
				return (0);
			game->j++;
		}
		game->i++;
	}
	if (collec['C'] == 0 || collec['E'] != 1 || collec['P'] != 1)
			return (0); 
	game->collectibles = collec['C'];
	return (1);
}