/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 10:36:22 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/08 14:32:34 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdio.h>

int	is_open(char *filename, t_game *game)
{
	game->fd = open(filename, O_RDONLY);
	return (game->fd > 0);
}

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

void mlx_map_destroyer(t_game *game)
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

int close_game(t_game *game)
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



void player_moves(int i, int j, t_game *game)
{
    static size_t move;
    char p;

    count_collected(game);
    p = game->map[game->player_y + i][game->player_x + j];
    if (p == 'C')
        game->collectibles--;
    if ((p != '1' && p != 'E') || (p == 'E' && !game->collectibles))
    {
        move++;
        game->moves = move;
        printf("Move count: %zu\n", move);
        game->map[game->player_y][game->player_x] = '0';
        game->map[game->player_y + i][game->player_x + j] = 'P';
        game->player_y += i;
        game->player_x += j;
        draw_map(game);
        if (p == 'E' && !game->collectibles)
        {
            write(1, "ela slametak . nadiii !!!\n", 24);
            mlx_map_destroyer(game);
        }
    }
}


int key_press(int keycode, t_game *game)
{
    printf("Key Pressed: %d\n", keycode);
    int i = 0; 
    int j = 0; 

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


void draw_map(t_game *game)
{
    mlx_clear_window(game->mlxs.mlx, game->mlxs.mlx_win);

    for (game->i = 0; game->map[game->i]; game->i++)
    {
        for (game->j = 0; game->map[game->i][game->j]; game->j++)
        {
            if (game->map[game->i][game->j] == 'E')
                mlx_put_image_to_window(game->mlxs.mlx, game->mlxs.mlx_win, game->exit, game->j * 40, game->i * 40);
            else if (game->map[game->i][game->j] == 'P')
                mlx_put_image_to_window(game->mlxs.mlx, game->mlxs.mlx_win, game->player, game->j * 40, game->i * 40);
            else if (game->map[game->i][game->j] == '1')
                mlx_put_image_to_window(game->mlxs.mlx, game->mlxs.mlx_win, game->wall, game->j * 40, game->i * 40);
            else if (game->map[game->i][game->j] == 'C')
                mlx_put_image_to_window(game->mlxs.mlx, game->mlxs.mlx_win, game->coin, game->j * 40, game->i * 40);
        }
    }
}


void xpm_to_img(t_game *game)
{
    game->player = mlx_xpm_file_to_image(game->mlxs.mlx, "images/player.xpm", &game->img_width, &game->img_height);
    game->exit = mlx_xpm_file_to_image(game->mlxs.mlx, "images/exit.xpm", &game->img_width, &game->img_height);
    game->wall = mlx_xpm_file_to_image(game->mlxs.mlx, "images/wall.xpm", &game->img_width, &game->img_height);
    game->coin = mlx_xpm_file_to_image(game->mlxs.mlx, "images/collectible.xpm", &game->img_width, &game->img_height);

    if (!game->player || !game->exit || !game->wall || !game->coin)
    {
        write(2, "Error\nimage name is not compatible!\n", 35);
        mlx_map_destroyer(game);
    }
}
void render_map(t_game *game)
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

// void ff()
// {
//     system("leaks so_long");
// }
int	main(int argc, char const *argv[])
{
    //atexit(ff);
	t_game	*game;

	if (argc == 2)
	{
		if (ft_strcmp(ft_strrchr(argv[1], '.'), ".ber") == 0)
		{
			game = (t_game *)malloc(sizeof(t_game));
			if (!game)
				print_error("memory allocation failed\n");
			parse_game(argv[1], game);
            render_map(game);
			free_game(game);
		}
		else
			print_error("file must have the extension .ber\n");
	}
	else
		print_error("number of arguments must be 2!\n");
	return (0);
}