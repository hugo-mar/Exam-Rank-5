// #define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct s_bsq
{
	int     h;
	int     w;
	char    empty;
	char    obstacle;
	char    full;
	char    **grid;
}   t_bsq;

void ft_free(char **grid)
{
	if (!grid)						// Importante check inicial
		return;

	int i = 0;
	while (grid[i])
		free(grid[i++]);
	free(grid);
}

size_t ft_strlen(char *str)
{
	size_t len = 0;				// size_t

	while (str && str[len])
		++len;

	return len;
}

char *ft_strdup(char *str)
{
	if (!str)
		return NULL;

	size_t len = ft_strlen(str);
	char *dup = malloc(len + 1);
	if (!dup)
		return NULL;

	for (size_t i = 0; i <= len; ++i)
		dup[i] = str[i];

	return dup;
}

static bool is_printable(unsigned char c)		// unsigned
{
    return (c >= 32 && c <= 126);
}

bool read_header(FILE *in, t_bsq *b)
{
	if (fscanf(in, "%d %c %c %c", &b->h, &b->empty, &b->obstacle, &b->full) != 4)
		return false;
	if (b->h < 1)
		return false;
	if (!is_printable(b->empty) || !is_printable(b->obstacle) || !is_printable(b->full))
		return false;
	if (b->empty == b->obstacle || b->empty == b->full || b->obstacle == b->full)
		return false;

	int c;
	while ((c = fgetc(in)) != '\n')
	{
		if (c == EOF)
			return false;
	}
	return true;
}

bool alloc_grid(t_bsq *b)
{
	b->grid = malloc(sizeof(char *) * (b->h + 1));
	if (!b->grid)
		return false;

	for (int i = 0; i <= b->h; ++i)
		b->grid[i] = NULL;
	
	return true;
}

bool read_grid(FILE *in, t_bsq *b, char **line, size_t *cap)	// Cap is size_t
{
	ssize_t n;

	b->w = 0;
	for (int i = 0; i < b->h; ++i)
	{
		n = getline(line, cap, in);
		if (n == -1)
			return false;

		if (n < 2 || (*line)[n - 1] != '\n')	/* cada linha tem de terminar em '\n' e conter pelo menos 1 célula */
			return false;

		int w = (int)(n - 1);					/* largura útil do mapa (exclui o newline final) */

		if (b->w == 0)							/* a primeira linha define a largura do mapa */
			b->w = w;
		else if (b->w != w)						/* ...as seguintes têm de ter o mesmo tamanho */
			return false;

		b->grid[i] = ft_strdup(*line);			/* duplicar a linha lida */
		if (!b->grid[i])
			return false;
		
		b->grid[i][w] = '\0';					/* remover o '\n' final para facilitar validação e DP */

	}

	n = getline(line, cap, in);
	if (n != -1)
		return false;
		
	return true;
}

bool validate_chars(t_bsq* b)
{
	for (int y = 0; y < b->h; ++y)
		for  (int x = 0; x < b->w; ++x)
			if (b->grid[y][x] != b->empty && b->grid[y][x] != b->obstacle)
				return false;
	return true;
}

void map_error_cleanup(t_bsq *bsq, char *line)
{
	ft_free(bsq->grid);
	free(line);
	fputs("map error\n", stderr);
}

void draw_square(t_bsq* b, int top_y, int top_x, int s)
{
	for (int y = top_y; y < top_y + s; ++y)
		for (int x = top_x; x < top_x + s; ++x)
			b->grid[y][x] = b->full;
}

void print_grid(t_bsq* b)
{

	for (int y = 0; y < b->h; ++y) {
		fputs(b->grid[y], stdout);
		fputs("\n", stdout);
	}
}

int min3(int top, int left, int diag)
{
	int min = top;
	if (left < min)	min = left;
	if (diag < min)	min = diag;

	return min;
}

bool solve_bsq_dp(t_bsq* b)
{
	int* prev = calloc((size_t)b->w, sizeof(int));
	int* curr = calloc((size_t)b->w, sizeof(int));
	if (!prev || !curr) {
		free(prev);
		free(curr);
		return false;
	}

	int best_s = 0;
	int best_top_y = 0;
	int best_top_x = 0;

	for (int y = 0; y < b->h; ++y)
	{
		for (int x = 0; x < b->w; ++x)
		{
			if (b->grid[y][x] == b->obstacle)
				curr[x] = 0;
			else if (x == 0 || y == 0)
				curr[x] = 1;
			else
				curr[x] = 1 + min3(prev[x], curr[x - 1], prev[x - 1]);

			int s = curr[x];
			if (s > 0)
			{
				int top_y = y - s + 1;
				int top_x = x - s + 1;
				
				if (s > best_s ||
					(s == best_s && (top_y < best_top_y ||
									(top_y == best_top_y && top_x < best_top_x))))
				{
					best_s = s;
					best_top_y = top_y;
					best_top_x = top_x;
				}
			}
		}
		for (int x = 0; x < b->w; ++x)
		{
			prev[x] = curr[x];
			curr[x] = 0;
		}
	}

	if (best_s > 0)
		draw_square(b, best_top_y, best_top_x, best_s);

	print_grid(b);

	free(prev);
	free(curr);

	return true;
}

void bsq(FILE *input)
{
	t_bsq  bsq = {0};
	char   *line = NULL;
	size_t cap = 0;

	if (!read_header(input, &bsq) ||
		!alloc_grid(&bsq) ||
		!read_grid(input, &bsq, &line, &cap) ||
		!validate_chars(&bsq) ||
		!solve_bsq_dp(&bsq))
	{
		map_error_cleanup(&bsq, line);
		return;
	}

	ft_free(bsq.grid);
	free(line);
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		bsq(stdin);
	}
	else
	{
		for (int i = 1; i < argc; ++i)
		{
			FILE* f = fopen(argv[i], "r");
			if (!f) 
				fputs("map error\n", stderr);
			else
			{
				bsq(f);
				fclose(f);
			}
			if (i < argc - 1)
				fputs("\n", stdout);
		}
	}
	return 0;
}