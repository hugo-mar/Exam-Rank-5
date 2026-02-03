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

	return true;
}

void apply_sketch(game_t *g)
{
	bool	draw = false;
	int		x = 0;
	int		y = 0;
	char	c;

	while (read(0, &c, 1) == 1)
	{
		if (c == 'w')
		{
			if (y > 0)
				--y;
		}
		else if (c == 'a')
		{
			if (x > 0)
				--x;
		}
		else if (c == 's')
		{
			if (y < g->h - 1)
				++y;
		}
		else if (c == 'd')
		{
			if (x < g->w - 1)
				++x;
		}
		else if (c == 'x')
			draw = !draw;

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
	int	n = 0;

	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; dx++)
		{
			if (dx == 0 && dy == 0)
				continue;

			int nx = x + dx;
			int ny = y + dy;

			if (nx < 0 || ny < 0 || nx >= g->w || ny >= g->h)
				continue;

			if (g->board[ny][nx] == 'O')
				++n;
		}
	}

	return n;
}

void simulate_life(game_t* g)
{
	for (int i = 0; i < g->it; ++i)
	{
		char** new_day = alloc_board(g->h, g->w);
		if (!new_day)
			return;

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
	simulate_life(&g);
	print_board(g.board);

	ft_free(g.board);
	return 0;
}