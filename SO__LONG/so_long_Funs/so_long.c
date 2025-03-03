#include "so_long.h"

void print_error(char *str)
{
    write (2, "Error\n", 6);
    write(2, str, ft_strlen(str));
    exit(1);
}

int is_open(char *filename, t_game *game)
{
    int fd = 0;
    game->fd = open(filename, O_RDONLY);
    if (fd > 0)
        game->fd = fd;
    return (game->fd > 0);
}

// void    reset_offset(t_game *game)
// {
//     game->fd = open("file name", O_RDONLY);
//     close (game->fd);
// }

int count_lines(t_game *game)
{
    int     lines;
    char    *line;
    int     fd;

    lines = 0;
    fd =  game->fd;
    line = get_next_line(fd);
    while (line)
    {
        lines++;
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return lines;
}

void free_map(char **map, int i)
{
    while (i >= 0)
    {
        free(map[i]);
        i--;
    }
    free(map);
}

void valid_map(t_game *game, char *line)
{
	size_t len;
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
    if (game->i > 0 && ft_strlen(game->map[game->i]) != ft_strlen(game->map[game->i - 1]))
    {
        print_error("map is not a rectangle\n");
        free(line);
        close(game->fd);
        free_map(game->map, game->i);
        return ;
    }
}

int is_closed(t_game *game)
{
	game->i = 0;
	game->j = 0;

	while (game->map[game->i][game->j])
	{
		if (game->map[game->i][game->j++] != '1')
			return (0);
	}
	game->i = game->map_height - 1;
	game->j = 0;
	while (game->map[game->i][game->j])
	{
		if (game->map[game->i][game->j++] != '1')
			return (0);
	}
	game->i = 1;
	game->j = game->map_width - 1;
	while (game->map[game->i] && game->map[game->i][0] == '1' && game->map[game->i][game->j] == '1')
		game->i++;
	if (game->map_height != game->i)
		return (0);
	return (1);
}

// void fill_map(t_game *game, char *line)
// {
// 	game->i = 0;
// 	size_t len;

// 	game->fd = open(line, O_RDONLY);
// 	len = game->map_width;
// 	while (len)
// 	{
// 		line = get_next_line(game->fd);
// 		game->map[i] = ft_substr(line, 0, game->map_width);
// 		game->map2[i] = ft_substr(line, 0, game->map_width);
// 		i++;
// 		len--;
// 	}
// 	game->map[i] = NULL;
// 	game->map2[i] = NULL;
// 	close(game->fd);
// }

void fill_map(t_game *game, char *line)
{
    game->i = 0;

    game->fd = open(line, O_RDONLY);
    while (game->i < game->map_height)
    {
        line = get_next_line(game->fd);
        if (!line) {
            print_error("Failed to read line from the map file\n");
            close(game->fd);
            return;
        }
        game->map2[game->i] = ft_substr(line, 0, game->map_width);
        if (!game->map2[game->i]) {
            print_error("Memory allocation failed for map2 line\n");
            free(line);
            close(game->fd);
            return;
        }
        free(line);
        game->i++;
    }
    game->map[game->i] = NULL;
    game->map2[game->i] = NULL;
    close(game->fd);
}


void check_map(t_game *game, char *line)
{
    game->map[game->i] = ft_strdup(line);
    if (!game->map[game->i])
    {
        print_error("Memory allocation failed for line\n");
        free(line);
        close(game->fd);
        free_map(game->map, game->i);
        exit(1);
    }
    valid_map(game, line);
	fill_map(game, line);
	// if (!is_closed(game))
	// 	print_error("The map is not surrounded by walls!");
}

void empty_line(t_game *game, char *line)
{
	if (!line)
    {
        print_error("map is empty\n");
        close(game->fd);
        free_map(game->map, game->i);
        return ;
    }
}
void read_map(char *filename, t_game *game)
{
    char    *line;
    int     lines;

    lines = count_lines(game);
    game->fd = open(filename, O_RDONLY);
    game->map = malloc((lines + 1) * sizeof(char *));
    if (!game->map)
    {
        print_error("Memory allocation failed\n");
        close(game->fd);
        return ;
    }
    game->i = 0;
    line = get_next_line(game->fd);
	empty_line(game, line);
    while (line)
    {
        check_map(game, line);
        free(line);
        game->i++;
        line = get_next_line(game->fd);
    }
    game->map[game->i] = NULL;
    close(game->fd);
	printf("map read successfully\n");
}

void print_game(t_game *game)
{
    printf("Game State:\n");
    printf("i: %d\n", game->i);
    printf("j: %d\n", game->j);
    printf("fd: %d\n", game->fd);
    //printf("filename: %s\n", game->filename);
    printf("Map Width: %d\n", game->map_width);
    printf("Map Height: %d\n", game->map_height);

    // Print the game map (2D array)
    printf("Map:\n");
    for (int row = 0; row < game->map_height; row++)
    {
        for (int col = 0; col < game->map_width; col++)
        {
            printf("%c", game->map[row][col]);
        }
        printf("\n");
    }
}

void ff()
{
    system("leaks so_long");
}
int main(int argc, char const *argv[])
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
            print_game(game);
            //printf("game->fd : %d\n", game->fd);
            //check_map(game, (char *)argv[1]);
            //free_map
        }
        else 
            print_error("file must have the extension .ber\n");
    }
    else
        print_error("number of arguments must be 2!\n");
    return 0;
}
