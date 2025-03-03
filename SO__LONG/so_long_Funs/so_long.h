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

#endif