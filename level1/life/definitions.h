#ifndef DEFINITIONS_H
# define DEFINITIONS_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>

# define SKETCH_MAX 1024

typedef struct game_s
{
	char	**board;
	int		w;
	int		h;
	int		it;
	char	*sketch;
	int		sk_size;
}		game_t;


#endif