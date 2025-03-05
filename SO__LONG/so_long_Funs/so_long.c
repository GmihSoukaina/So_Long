/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:22:47 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/05 21:22:26 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	is_open(char *filename, t_game *game)
{
	game->fd = open(filename, O_RDONLY);
	return (game->fd > 0);
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

void	player_position(t_game *game)
{
	game->i = 1;
	while (game->map[game->i])
	{
		game->j = 1;
		while (game->map[game->i][game->j])
		{
			if (game->map[game->i][game->j] == 'P')
			{
				game->player_x = game->j;
				game->player_y = game->i;
				return ;
			}
			game->j++;
		}
		game->i++;
	}
}
#include <stdio.h>

void print_game(t_game *game)
{
    int i;

    if (!game)
    {
        printf("Error: Game structure is NULL.\n");
        return;
    }

    printf("===== Game State =====\n");
    printf("i: %d\n", game->i);
    printf("j: %d\n", game->j);
    printf("fd: %d\n", game->fd);
    printf("Map Width: %d\n", game->map_width);
    printf("Map Height: %d\n", game->map_height);
    printf("Collectibles: %d\n", game->collectibles);
    printf("Player Position: (%d, %d)\n", game->player_x, game->player_y);

    if (game->map)
    {
        printf("\n===== Game Map (game->map) =====\n");
        for (i = 0; game->map[i] != NULL; i++)
        {
            printf("%s\n", game->map[i]);
        }
    }
    else
    {
        printf("\nGame Map (game->map) is NULL.\n");
    }
    if (game->map2)
    {
        printf("\n===== Game Map 2 (game->map2) =====\n");
        for (i = 0; game->map2[i] != NULL; i++)
        {
            printf("%s\n", game->map2[i]);
        }
    }
    else
    {
        printf("\nGame Map 2 (game->map2) is NULL.\n");
    }

    printf("\n===== End of Game State =====\n");
}

void parse_game(const char *filename, t_game *game)
{
    if (!is_open((char *)filename, game))
        print_error("file opening failed\n");
    read_map((char *)filename, game);
    if (!is_closed(game))
        print_error("map is not closed\n");
    if (!is_valid_rules(game))
        print_error("map is not valid 01CEP!");
    player_position(game);
}

int	main(int argc, char const *argv[])
{
	t_game	*game;

	if (argc == 2)
	{
		if (ft_strcmp(ft_strrchr(argv[1], '.'), ".ber") == 0)
		{
			game = (t_game *)malloc(sizeof(t_game));
			if (!game)
				print_error("memory allocation failed\n");
			if (!is_open((char *)argv[1], game))
				print_error("file opening failed\n");
			else
				read_map((char *)argv[1], game);
			if (!is_closed(game))
				print_error("map is not closed\n");
			if (!is_valid_rules(game))
				print_error("map is not valid 01CEP!");
			player_position(game);
			print_game(game);
			free_game(game);
		}
		else
			print_error("file must have the extension .ber\n");
	}
	else
		print_error("number of arguments must be 2!\n");
	return (0);
}
