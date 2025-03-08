/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 10:36:22 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/08 12:35:27 by sgmih            ###   ########.fr       */
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



void print_game(t_game *game)
{
    int i;

    if (!game)
    {
        printf("Error: Game structure is NULL.\n");
        return;
    }

    // Printing the mlx structure
    printf("===== MLX State =====\n");
    if (game->mlxs.mlx && game->mlxs.mlx_win)
    {
        printf("MLX: %p\n", game->mlxs.mlx);
        printf("MLX Window: %p\n", game->mlxs.mlx_win);
    }
    else
    {
        printf("MLX structure is not initialized.\n");
    }

    // Printing game state
    printf("===== Game State =====\n");
    printf("i: %d\n", game->i);
    printf("j: %d\n", game->j);
    printf("fd: %d\n", game->fd);
    printf("Map Width: %d\n", game->map_width);
    printf("Map Height: %d\n", game->map_height);
    printf("Collectibles: %d\n", game->collectibles);
    printf("Player Position: (%d, %d)\n", game->player_x, game->player_y);
    printf("Exit Position: (%d, %d)\n", game->exit_x, game->exit_y);

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
    p = game->map[game->player_x + i][game->player_y + j];

    printf("Current (game->player_x, game->player_y): (%d, %d) -> New (game->player_x + i, game->player_y + j): (%d, %d) | Value: %c\n",
           game->player_x, game->player_y, game->player_x + i, game->player_y + j, p);

    if (p == 'C')
        game->collectibles--;
    if ((p != '1' && p != 'E') || (p == 'E' && !game->collectibles))
    {
        move++;
        game->moves = move;
        printf("Move count: %zu\n", move);
        printf("Remaining Collectibles: %d\n", game->collectibles); 
        

        game->map[game->player_x][game->player_y] = '0';
        game->map[game->player_x + i][game->player_y + j] = 'P';


        game->player_x += i;
        game->player_y += j;


        draw_map(game);
        
        if (p == 'E' && !game->collectibles)
        {
            write(1, "You Win, Congrats !!!!!\n", 24);
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
        //mlx_map_destroyer(game);
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
    //mlx_loop_hook(game->mlxs.mlx, draw_map, game);
    mlx_loop(game->mlxs.mlx);
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