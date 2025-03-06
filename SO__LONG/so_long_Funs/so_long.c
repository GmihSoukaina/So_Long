/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:22:47 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/06 14:31:01 by sgmih            ###   ########.fr       */
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

void	exit_position(t_game *game)
{
    game->i = 1;
    while (game->map[game->i])
    {
        game->j = 1;
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
	flood_fill(game, game->player_y, game->player_x);
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

// void flood_fill(t_game *game, int y, int x)
// {
// 	if(game->map2[game->player_y][game->player_x] == '1' 
// 		|| game->map2[game->player_y][game->player_x] == 'E' 
// 		|| game->map2[game->player_y][game->player_x] == 'X')
// 		return ;
// 	if(game->map2[game->player_y][game->player_x] == 'C' 
// 		|| game->map2[game->player_y][game->player_x] == '0')
// 	{
// 		if (game->map2[game->player_y][game->player_x] == 'C')
//             game->collectibles--;
// 		game->map2[game->player_y][game->player_x] = 'X';
// 		// Recursively check all 4 directions
//         flood_fill(game, y + 1, x); // Down
//         flood_fill(game, y - 1, x); // Up
//         flood_fill(game, y, x + 1); // Right
//         flood_fill(game, y, x - 1); // Left
// 	}
// }

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

void	parse_game(const char *filename, t_game *game)
{
    if (!is_open((char *)filename, game))
        print_error("file opening failed\n");
    read_map((char *)filename, game);
    if (!is_closed(game))
        print_error("map is not closed\n");
    if (!is_valid_rules(game))
        print_error("map is not valid 01CEP!");
    player_position(game);
	flood_fill(game, game->player_y, game->player_x);
	exit_position(game);
	check_collectibles(game);
}

void	load_image(t_game *game, char *path)
{
	int	img_width;
	int	img_height;

	game->img = mlx_xpm_file_to_image(game->mlxs.mlx, path, &img_width, &img_height);
	if (!game->img)
		print_error("failed to load image\n");
}

void put_imgs_to_win(void *mlx, void *mlx_win, t_game *game)
{
    game->i = 0;
    game->j = 0;
    while (game->map[game->i])
    {
        game->j = 0;
        while (game->map[game->i][game->j])
        {
            if (game->map[game->i][game->j] == '1')
            {
                load_image(game, "./images/wall.xpm");
                mlx_put_image_to_window(mlx, mlx_win, game->img, game->j * 40, game->i * 40);
            }
            else if (game->map[game->i][game->j] == '0')
            {
                load_image(game, "./images/floor.xpm");
                mlx_put_image_to_window(mlx, mlx_win, game->img, game->j * 40, game->i * 40);
            }
            else if (game->map[game->i][game->j] == 'C')
            {
                load_image(game, "./images/collectible.xpm");
                mlx_put_image_to_window(mlx, mlx_win, game->img, game->j * 40, game->i * 40);
            }
            else if (game->map[game->i][game->j] == 'E')
            {
                load_image(game, "./images/exit.xpm");
                mlx_put_image_to_window(mlx, mlx_win, game->img, game->j * 40, game->i * 40);
            }
            else if (game->map[game->i][game->j] == 'P')
            {
                load_image(game, "./images/player.xpm");
                mlx_put_image_to_window(mlx, mlx_win, game->img, game->j * 40, game->i * 40);
            }
            game->j++;
        }
        game->i++;
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
			parse_game(argv[1], game);
			print_game(game);

            game->mlxs.mlx = mlx_init();
            
            //game->mlxs.mlx_win = mlx_new_window(game->mlxs.mlx, game->map_width * 40, game->map_height * 40, "so_long");
            game->mlxs.mlx_win = mlx_new_window(game->mlxs.mlx, 800, 800, "so_long");


            load_image(game, "./images/tanjiro.xpm");

			// Put image on window at (x=0, y=0)
            put_imgs_to_win(game->mlxs.mlx, game->mlxs.mlx_win, game);
			//mlx_put_image_to_window(game->mlxs.mlx, game->mlxs.mlx_win, game->img, 0, 0);

            mlx_loop(game->mlxs.mlx);
            
			free_game(game);
            //mlx init 
            //mlx creat new window
            //mlx xpm file to img 
            //mlx put img to window
            //mlx loop
            
		}
		else
			print_error("file must have the extension .ber\n");
	}
	else
		print_error("number of arguments must be 2!\n");
	return (0);
}
