#include "so_long.h"

void print_error(char *str)
{
    write (2, "Error\n", 6);
    write(2, str, ft_strlen(str));
    exit(1);
}

int open_file(char *file)
{
    int		fd;
	int		count;
	char	*line;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		print_error("Error\nfile cannot be opened!\n");
	count = 0;
	line = get_next_line(fd);
    while (line)
    {
        count++;
        free(line);
        line = get_next_line(fd);
    }
    free(line);
    close(fd);
    return (count);
}

int main(int argc, char const *argv[])
{
    t_game *game;
    if (argc == 2)
    {
        if (ft_strcmp(ft_strrchr(argv[1], '.'), ".ber") != 0)
        {
            game = (t_game *)malloc(sizeof(t_game));
            if (!game)
                print_error("memory allocation failed\n");
            if (!open_file(argv[1]))
                print_error("file opening failed\n");
        }
        else 
            print_error("file must have the extension .ber\n");
    }
    else
        print_error("number of arguments must be 2!\n");
    return 0;
}
