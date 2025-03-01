#include "so_long.h"

void print_error(char *str)
{
    write (2, "Error\n", 6);
    write(2, str, ft_strlen(str));
    exit(1);
}

int is_open(char *file, t_game *game)
{
    int fd = 0;
    game->fd = open(file, O_RDONLY);
    if (fd > 0)
        game->fd = fd;
    printf("fd = %d\n", game->fd);
    return (game->fd > 0);
}

// void read_map(char *file, t_game *game)
// {
//     char *line;

//     game->fd = open(file, O_RDONLY);
// 	if (game->fd == -1)
// 		print_error("Error\nfile cannot be opened!\n");
//     game->i = 0;
//     line = get_next_line(game->fd);
//     while (line)
//     {
//         if ((ft_strcmp(line, "\n") == 0) || ft_strlen(line) == 0)
//         {
//             free(line);
//             close(game->fd);
//             print_error("map is invalid\n");
//         }
//         game->map[game->i] = ft_strdup(line);
//         free(line);
//         if (game->map[game->i][ft_strlen(game->map[game->i]) - 1] == '\n')
//             game->map[game->i][ft_strlen(game->map[game->i]) - 1] = '\0';
//         game->i++;
//         line = get_next_line(game->fd);
//     }
//     game->map[game->i] = NULL;
//     close(game->fd);
// }

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
            printf("file name: %s\n", argv[1]);

            // check if file is open
            if (!is_open((char *)argv[1] , game))
                print_error("file opening failed\n");
            else
                printf("file opened successfully\n");
                //read_map((char *)argv[1] , game);
        }
        else 
            print_error("file must have the extension .ber\n");
    }
    else
        print_error("number of arguments must be 2!\n");
    return 0;
}
