/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:22:47 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/04 14:01:04 by sgmih            ###   ########.fr       */
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
void print_game(t_game *game)
{
	int i;
	
	printf("Game State:\n");
	printf("i: %d\n", game->i);
	printf("j: %d\n", game->j);
	printf("fd: %d\n", game->fd);
	//printf("filename: %s\n", game->filename);
	printf("Map Width: %d\n", game->map_width);
	printf("Map Height: %d\n", game->map_height);

	i = 0;
	printf("Game Map (game->map):\n");
	while (game->map[i] != NULL)
	{
		printf("%s\n", game->map[i]);
		i++;
	}
	i = 0;
	printf("\nGame Map 2 (game->map2):\n");
	while (game->map2[i] != NULL)
	{
		printf("%s\n", game->map2[i]);
		i++;
	}
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
