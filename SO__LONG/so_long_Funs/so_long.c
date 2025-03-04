/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:22:47 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/04 11:59:11 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	print_error(char *str)
{
	write (2, "Error\n", 6);
	write(2, str, ft_strlen(str));
	exit(1);
}

int	is_open(char *filename, t_game *game)
{
	game->fd = open(filename, O_RDONLY);
	return (game->fd > 0);
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

int check_walls(t_game *game)
{
	// Check top wall
	game->i = 0;
	game->j = 0;
	while (game->map[game->i][game->j])
	{
		if (game->map[game->i][game->j++] != '1')
			return (0);
	}

	// Check bottom wall
	game->i = game->map_height - 1;
	game->j = 0;
	while (game->map[game->i][game->j])
	{
		if (game->map[game->i][game->j++] != '1')
			return (0);
	}

	// Check left & right walls
	game->i = 1;
	game->j = game->map_width - 1;
	while (game->i < game->map_height)
	{
		if (game->map[game->i][0] != '1' || game->map[game->i][game->j] != '1')
			return (0);
		game->i++;
	}
	
	// If the height does not match, the map is not closed
	if (game->map_height != game->i)
		return (0);

	return (1);
}


int is_closed(t_game *game)
{
	// Calculate map height
	game->map_height = 0;
	while (game->map[game->map_height])
		game->map_height++;

	// Calculate map width using the first row
	game->map_width = ft_strlen(game->map[0]);

	// Call check_walls to validate wall boundaries
	if (!check_walls(game))
	{
		print_error("map is not closed\n");
		free_map(game->map, game->map_height);
		free_map(game->map2, game->map_height);
		exit(1);
	}

	return (1);
}



// int is_closed(t_game *game)
// {
// 	game->map_height = 0;
// 	while (game->map[game->map_height])
// 		game->map_height++;
// 	game->map_width = ft_strlen(game->map[0]);
// 	game->i = 0;
// 	game->j = 0;
// 	while (game->map[game->i][game->j])
// 	{
// 		if (game->map[game->i][game->j++] != '1')
// 			return (0);
// 	}
// 	game->i = game->map_height - 1;
// 	game->j = 0;
// 	while (game->map[game->i][game->j])
// 	{
// 		if (game->map[game->i][game->j++] != '1')
// 			return (0);
// 	}
// 	game->i = 1;
// 	game->j = game->map_width - 1;
// 	while (game->i < game->map_height)
// 	{
// 		if (game->map[game->i][0] != '1' || game->map[game->i][game->j] != '1')
// 			return (0);
// 		game->i++;
// 	}
// 	if (game->map_height != game->i)
// 		return (0);
// 	return (1);
// }


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

void ff()
{
	system("leaks so_long");
}
int	main(int argc, char const *argv[])
{
	//atexit(ff);
	t_game *game;
	if (argc == 2)
	{
		if (ft_strcmp(ft_strrchr(argv[1], '.'), ".ber") == 0)
		{
			game = (t_game *)malloc(sizeof(t_game));
			if (!game)
				print_error("memory allocation failed\n");
			if (!is_open((char *)argv[1] , game))
				print_error("file opening failed\n");
			else
				read_map((char *)argv[1] , game);
			if (!is_closed(game))
				print_error("map is not closed\n");
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
