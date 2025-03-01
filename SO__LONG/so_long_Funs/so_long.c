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

int count_lines(char *filename)
{
    int lines = 0;
    char *line;
    int fd = open(filename, O_RDONLY);

    if (fd == -1) {
        print_error("Failed to open map file for counting lines\n");
        return 0;
    }

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


void read_map(char *filename, t_game *game)
{
    char    *line;
    size_t  len;
    int     lines;

    game->fd = open(filename, O_RDONLY);
    if (game->fd == -1) {
        print_error("Failed to open map file\n");
        return ;
    }

    game->i = 0;

    lines = count_lines(filename);
    printf("lines: %d\n", lines);
    game->map = malloc((lines + 1) * sizeof(char *));
    if (!game->map) {
        print_error("Memory allocation failed\n");
        close(game->fd);
        return ;
    }

    line = get_next_line(game->fd);
    while (line)
    {
        if ((ft_strcmp(line, "\n") == 0) || ft_strlen(line) == 0)
        {
            print_error("map is invalid\n");
            free(line);
            close(game->fd);
        }

        // Allocate memory for each line in map
        game->map[game->i] = ft_strdup(line);
        if (!game->map[game->i]) {
            print_error("Memory allocation failed for line\n");
            free(line);
            close(game->fd);
        }

        printf("%s\n", game->map[game->i]);

        len = ft_strlen(game->map[game->i]);
        if (len > 0 && game->map[game->i][len - 1] == '\n')
            game->map[game->i][len - 1] = '\0';

        free(line);

        game->i++;
        line = get_next_line(game->fd);
    }
    // Null-terminate the map array
    game->map[game->i] = NULL;
    close(game->fd);
    printf("Map read successfully\n");
}

int valid_map(t_game *game);

int chech_map(t_game *game, char *filename)
{
    if (valid_map(game))
        print_error("map is invalid\n");
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
            check_map(game, argv[1]);
            //free_map
        }
        else 
            print_error("file must have the extension .ber\n");
    }
    else
        print_error("number of arguments must be 2!\n");
    return 0;
}
