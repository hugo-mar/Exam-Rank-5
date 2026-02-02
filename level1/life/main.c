#include "definitions.h"

int	ft_putstr(char *str)
{
	int len = 0;

	if (!str)
		return len;

	while (str[len])
		putchar(str[len++]);

	return len;
}

void ft_free(char** board)
{
	if (!board)
		return;
	int y = 0;
	while (board[y])
		free(board[y++]);
	free(board);
}

char** alloc_board(int h, int w)
{
	char** board = malloc(sizeof(char*) * (h + 1));
	if (!board) {
		return NULL;
	}

	for (int y = 0; y <= h; ++y)
		board[y] = NULL;	
	
	for (int y = 0; y < h; ++y)
	{
		board[y] = malloc(sizeof(char) * (w + 1));
		if (!board[y]) {
			ft_free(board);
			return NULL;
		}
		for (int x = 0; x < w; ++x)
			board[y][x] = ' ';
		board[y][w] = '\0';
	}

	return board;
}

bool init_game(char **argv, game_t *g)
{
	g->w = atoi(argv[1]);
	g->h = atoi(argv[2]);
	g->it = atoi(argv[3]);
	
	if (g->w <= 0 || g->h <= 0 || g->it < 0)
	return false;
	
	g->board = alloc_board(g->h, g->w);
	if (!g->board)
	return false;
	
	g->sketch = malloc(SKETCH_MAX);
	if (!g->sketch) {
		ft_free(g->board);
		return false;
	}
		
	g->sk_size = read(0, g->sketch, SKETCH_MAX -1);
	if (g->sk_size <= 0)
		g->sketch[0] = '\0';
	else
		g->sketch[g->sk_size] = '\0';

	return true;
}

void apply_sketch(game_t* g)
{
	bool	draw = false;
	int		x = 0;
	int 	y = 0;

	for (int i = 0; g->sketch[i]; ++i) {

		switch (g->sketch[i])
		{
		case 'w':
			if (y > 0)
				--y;
			break;
		case 'a':
			if (x > 0)
				--x;
			break;		
		case 's':
			if (y < g->h - 1)
				++y;
			break;
		case 'd':
			if (x < g->w -1)
				++x;
			break;
		case 'x':
			draw = !draw;
			break;
		default:
			break;
		}

		if (draw)
			g->board[y][x] = 'O';
	}
}

void print_board(char** board)
{
	for (int y = 0; board[y]; ++y) {
		ft_putstr(board[y]);
		putchar('\n');
	}
}

int count_neighbours(game_t* g, int x, int y)
{
	int	neighbours = 0;

	if (x != 0 && y != 0 && g->board[y - 1][x - 1] == 'O') ++neighbours;
	if (y != 0 && g->board[y - 1][x] == 'O') ++neighbours;
	if (x + 1 != g->w && y != 0 && g->board[y - 1][x + 1] == 'O') ++neighbours;

	if (x != 0 && g->board[y][x - 1] == 'O') ++neighbours;
	if (x + 1 != g->w && g->board[y][x + 1] == 'O') ++neighbours;

	if (x != 0 && y + 1 != g->h && g->board[y + 1][x - 1] == 'O') ++neighbours;
	if (y + 1 != g->h && g->board[y + 1][x] == 'O') ++neighbours;
	if (x + 1 != g->w && y + 1 != g->h && g->board[y + 1][x + 1] == 'O') ++neighbours;

	return neighbours;
}

void iterate_life(game_t* g)
{
	for (int i = 0; i < g->it; ++i)
	{
		char** new_day = alloc_board(g->h, g->w);
		if (!new_day)
			return ;
		for (int y = 0; y < g->h; ++y)
		{
			for (int x = 0; x < g->w; ++x)
			{
				int n = count_neighbours(g, x, y);
				if (g->board[y][x] == 'O')
					 new_day[y][x] = (n == 2 || n == 3) ? 'O' : ' ';
				else
					new_day[y][x] = (n == 3) ? 'O' : ' ';
			}
		}
		ft_free(g->board);
		g->board = new_day;
	}
}

int main (int argc, char **argv)
{
	game_t	g = {0};

	if (argc != 4 || init_game(argv, &g) == false)
		return 1;
	
	apply_sketch(&g);
	iterate_life(&g);
	print_board(g.board);

	ft_free(g.board);
	free(g.sketch);
	return 0;
}