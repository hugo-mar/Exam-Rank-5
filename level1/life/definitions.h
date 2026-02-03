#ifndef DEFINITIONS_H
# define DEFINITIONS_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>

typedef struct game_s
{
	char	**board;
	int		w;
	int		h;
	int		it;
}		game_t;


#endif