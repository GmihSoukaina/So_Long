/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:22:47 by sgmih             #+#    #+#             */
/*   Updated: 2025/03/07 11:30:52 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	is_open(char *filename, t_game *game)
{
	game->fd = open(filename, O_RDONLY);
	return (game->fd > 0);
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

void	set_background(t_game *game, char *path)
{
	game->height = 1200;
	game->width = 1200;
	game->img_width = 1200;
	game->img_height = 1200;
	void *bg_img;
	int	x;
	int	y;

	bg_img = mlx_xpm_file_to_image(game->mlxs.mlx, path, &game->img_width, &game->img_height);
	if (!bg_img)
		print_error("failed to load background image\n");
	y = 0;
	while (y < game->height)
	{
		x = 0;
		while (x < game->width)
		{
			mlx_put_image_to_window(game->mlxs.mlx, game->mlxs.mlx_win, bg_img, x, y);
			x += game->img_width;
		}
		y += game->img_height;
	}
}
int	destroy(t_game *game)
{
	free_game(game);
	mlx_destroy_window(game->mlxs.mlx, game->mlxs.mlx_win);
	exit(0);
    return (0);
}

void	move_player(int i, int j, t_game *game)
{
	static size_t	move;
	char			p;

	p = game->map[game->player_x + i][game->player_y + j];
	if (p == 'C')
		game->collectibles--;
	if ((p != '1' && p != 'E') || (p == 'E' && game->collectibles == 0))
	{
		move++;
		if (p == 'E')
		{
			write(1, "ela slamtak, Naaadiii !!!!!\n", 27);
			destroy(game);
		}
		game->map[game->player_y][game->player_x] = '0';
        game->player_x += i;
        game->player_y += j;
        game->map[game->player_y][game->player_x] = 'P';
		put_imgs_to_win(game->mlxs.mlx, game->mlxs.mlx_win, game);
	}
}


int key_press(int keycode, t_game *game)
{
    if (keycode == 53) // Escape key to quit
        destroy(game);
    else if (keycode == 126) // Up arrow key
        move_player(0, -1, game);
    else if (keycode == 123) // Left arrow key
        move_player(-1, 0, game);
    else if (keycode == 125) // Down arrow key
        move_player(0, 1, game);
    else if (keycode == 124) // Right arrow key
        move_player(1, 0, game);
    return (0);
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
            game->mlxs.mlx_win = mlx_new_window(game->mlxs.mlx, 1200, 1200, "so_long");
            set_background(game, "./images/background.xpm");
            put_imgs_to_win(game->mlxs.mlx, game->mlxs.mlx_win, game);

            mlx_key_hook(game->mlxs.mlx_win, key_press, game);
            mlx_hook(game->mlxs.mlx_win, 17, 0, destroy, game);

            
            mlx_loop(game->mlxs.mlx);
			free_game(game);
		}
		else
			print_error("file must have the extension .ber\n");
	}
	else
		print_error("number of arguments must be 2!\n");
	return (0);
}
