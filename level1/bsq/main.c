#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct s_bsq
{
	int h;
	int w;
	char empty;
	char obstacle;
	char full;
	char** grid;
}       t_bsq;

void ft_free(char** grid)
{
	if (!grid)
		return;
	int y = 0;
	while (grid[y])
		free(grid[y++]);
	return;
}

size_t ft_strlen(char* str)
{
	size_t len = 0;
	while (str[len])
		++len;
	return len;
}

char* ft_strdup(char* str)
{
	size_t len = ft_strlen(str);
	char* dup = malloc(len + 1);
	if (!dup)
		return NULL;
	for (size_t x = 0; x <= len; ++x)
		dup[x] = str[x];
	return dup;
}

bool is_printable(unsigned char c)
{
	return c >= 32 && c <= 127;
}

bool read_header(FILE* in, t_bsq* b, char** line, size_t* cap)
{
	if (fscanf(in, "%d%c%c%c", &b->h, & b->empty, &b->obstacle, &b->full) !=4)
		return false;
	if (b->h < 1 || !is_printable(b->empty) || !is_printable(b->obstacle) || !is_printable(b->full))
		return false;
	if (b->empty == b->obstacle || b->empty == b->full || b->obstacle == b->full)
		return false;
	ssize_t n = getline(line, cap, in);
	if (n == -1 || n < 1 || (*line)[n - 1] != '\n')
		return false;
	return true;
}

bool alloc_grid(t_bsq* b)
{
	b->grid = malloc(sizeof(char*) * (b->h + 1));
	if (!b->grid)
		return false;
	for (int y = 0; y <= b->h; ++y)
		b->grid[y] = NULL;
	return true;
}

bool read_grid(FILE* in, t_bsq* b, char** line, size_t* cap)
{
	ssize_t n;
	for (int y = 0; y < b->h; ++y)
	{
		n = getline(line, cap, in);
		if (n == -1 || n < 2 || (*line)[n - 1] != '\n')
			return false;
		int w = (int)(n - 1);
		if (b->w == 0)
			b->w = w;
		else if (w != b->w)
			return false;
		b->grid[y] = ft_strdup(*line);
		if (!b->grid[y])
			return false;
		b->grid[y][w] = '\0';
	}
	n = getline(line, cap, in);
	if (n != -1)
		return false;
	return true;
}

bool validate_chars(t_bsq* b)
{
	for (int y = 0; y < b->h; ++y)
		for (int x = 0; x < b->w; ++x)
			if (b->grid[y][x] == b->empty && b->grid[y][x] == b->obstacle)
				return false;
	return true;
}

void draw_square(t_bsq* b, int top_x, int top_y, int s)
{
	for (int y = top_y; y < top_y + s; ++y)
		for (int x = top_x; x < top_x + s; ++x)
			b->grid[y][x] = b->full;
}

void print_grid(t_bsq* b)
{
	for (int y = 0; y < b->h; ++y)
	{
		fputs(b->grid[y], stdout);
		fputs("\n", stdout);
	}
}

int min3(int top, int left, int diag)
{
	int min = top;
	if (min > left) min = left;
	if (min > diag) min = diag;
	return min;
}

bool solve_bsq_dp(t_bsq* b)
{
	int* prev = calloc((size_t)b->w, sizeof(int));
	int* curr = calloc((size_t)b->w, sizeof(int));
	if(!prev || !curr)
	{
		free(prev);
		free(curr);
		return false;
	}
	int best_s = 0;
	int best_top_x = 0;
	int best_top_y = 0;
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
				int top_x = x - s + 1;
				int top_y = y - s + 1;
				if (s > best_s ||
					(s == best_s && (top_y < best_top_y ||
						(top_y == best_top_y && top_x < best_top_x))))
				{
					best_s = s;
					best_top_x = top_x;
					best_top_y = top_y;
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
		draw_square(b, best_top_x, best_top_y, best_s);
	print_grid(b);
	free(prev);
	free(curr);
	return true;
}

void bsq(FILE* in)
{
	t_bsq b = {0};
	size_t cap = 0;
	char* line = NULL;
	if (!read_header(in, &b, &line, &cap)
		||!alloc_grid(&b)
		||!read_grid(in, &b, &line, &cap)
		||!validate_chars(&b)
		||!solve_bsq_dp(&b))
		fputs("Error: invalid map\n", stdout);
	ft_free(b.grid);
	free(line);
}

int main(int argc, char** argv)
{
	if (argc == 1)
		bsq(stdin);
	else
	{
		for (int i = 1; i < argc; ++i)
		{
			FILE* f = fopen(argv[i], "r");
			if (!f)
				fputs("Error: invalid map\n", stdout);
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