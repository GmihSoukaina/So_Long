#ifndef SO_LONG_H
# define SO_LONG_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "../lib/libft/libft.h"
# include "../lib/gnl/get_next_line.h"

typedef struct s_game
{
    int     i;
    int     j;
    int     fd;
    char    **map; // Game Map (2D array)
    char    **map2; // Game Map (2D array)
    int	    map_width;    // Map width
	int	    map_height;   // Map height 
} t_game;

void    read_map(char *filename, t_game *game);
void    check_map(t_game *game, char *line);
void	valid_map(t_game *game, char *line);
int     is_closed(t_game *game);
int     check_walls(t_game *game);
void    empty_line(t_game *game, char *line);
void    free_map(char **map, int height);
void	free_game(t_game *game);
void    print_error(char *error);
int     count_lines(t_game *game);
int     is_open(char *filename, t_game *game);
#endif