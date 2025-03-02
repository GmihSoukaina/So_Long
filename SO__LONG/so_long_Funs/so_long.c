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
//     close (game->fd);
//     game->fd = open("file name", O_RDONLY);
// }

int count_lines(t_game *game)
{
    int lines = 0;
    char *line;
    reset_offset(game);

    printf("fd : %d\n", game->fd);

    line = get_next_line(game->fd);
    while (line)
    {
        lines++;
        free(line);
        line = get_next_line(game->fd);
    }
    close(game->fd);
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

void read_map(char *filename, t_game *game)
{
    (void)filename;
    char    *line;
    //size_t  len;
    int     lines;

    // game->fd = open(filename, O_RDONLY);
    // if (game->fd == -1) {
    //     print_error("Failed to open map file\n");
    //     return ;
    // }
    game->i = 0;
    lines = count_lines(game);
    game->map = malloc((lines + 1) * sizeof(char *));
    if (!game->map) {
        print_error("Memory allocation failed\n");
        close(game->fd);
        return ;
    }
    line = get_next_line(game->fd);
    while (line)
    {
        game->map[game->i] = ft_strdup(line);
        if (!game->map[game->i]) {
            print_error("Memory allocation failed for line\n");
            free(line);
            close(game->fd);
            free_map(game->map, game->i);
        }
        printf("line : %s\n", line);
        printf("game->map[%d] : %s\n", game->i, game->map[game->i]);

        // if (!line || (ft_strcmp(line, "\n") == 0) || ft_strlen(line) == 0)
        // {
        //     print_error("map is invalid\n");
        //     free(line);
        //     close(game->fd);
        //     free_map(game->map, game->i);
        // }

        // len = ft_strlen(game->map[game->i]);
        // if (len > 0 && game->map[game->i][len - 1] == '\n')
        //     game->map[game->i][len - 1] = '\0';

        
        // if (game->i > 0 && ft_strlen(game->map[game->i]) != ft_strlen(game->map[game->i - 1]))
        // {
        //     print_error("map is not a rectangle\n");
        //     free(line);
        //     close(game->fd);
        //     free_map(game->map, game->i);
        // }

        free(line);
        game->i++;
        line = get_next_line(game->fd);
    }
    game->map[game->i] = NULL;
    close(game->fd);
}

void print_game(t_game *game)
{
    printf("Game State:\n");
    printf("i: %d\n", game->i);
    printf("j: %d\n", game->j);
    printf("fd: %d\n", game->fd);
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


int main(int argc, char const *argv[])
{
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
