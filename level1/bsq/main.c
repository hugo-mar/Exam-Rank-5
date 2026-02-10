#define _GNU_SOURCE
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
	if (!grid)
		return;

	int i = 0;
	while (grid[i])
		free(grid[i++]);
	free(grid);
}

size_t ft_strlen(char *str)
{
	size_t len = 0;

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

static bool is_printable(char c)
{
    return (c >= 32 && c <= 126);
}

bool read_header(FILE *in, t_bsq *bsq)
{
	if (fscanf(in, "%d %c %c %c\n", &bsq->h, &bsq->empty, &bsq->obstacle, &bsq->full) != 4)
		return false;
	if (bsq->h < 1)
		return false;
	if (!is_printable(bsq->empty) || !is_printable(bsq->obstacle) || !is_printable(bsq->full))
		return false;
	if (bsq->empty == bsq->obstacle || bsq->empty == bsq->full || bsq->obstacle == bsq->full)
		return false;

	return true;
}

bool alloc_grid(t_bsq *bsq)
{
	bsq->grid = malloc(sizeof(char *) * (bsq->h + 1));
	if (!bsq->grid)
		return false;

	for (int i = 0; i <= bsq->h; ++i)
		bsq->grid[i] = NULL;
	
	return true;
}

bool read_grid(FILE *in, t_bsq *bsq, char **line, size_t *cap)
{
	ssize_t n;

	bsq->w = 0;
	for (int i = 0; i < bsq->h; ++i)
	{
		n = getline(line, cap, in);
		if (n == -1)
			return false;

		if (n < 2 || (*line)[n - 1] != '\n')	/* cada linha tem de terminar em '\n' e conter pelo menos 1 célula */
			return false;

		int w = (int)(n - 1);					/* largura útil do mapa (exclui o newline final) */
		if (w < 1)
			return false;

		bsq->grid[i] = ft_strdup(*line);		/* duplicar a linha lida */
		if (!bsq->grid[i])
			return false;

		bsq->grid[i][w] = '\0';					/* remover o '\n' final para facilitar validação e DP */

		
		if (bsq->w == 0)						/* a primeira linha define a largura do mapa */
			bsq->w = w;
		else if (bsq->w != w)					/* ...as seguintes têm de ter o mesmo tamanho */
			return false;
	}
	return true;
}


bool check_eof(FILE *in, char **line, size_t *cap)
{
	ssize_t n;

	while ((n = getline(line, cap, in)) != -1)
	{
		if (n == 1 && (*line)[0] == '\n')		/* ignora linhas vazias extra */
			continue;

		return false;							/* se tiver qualquer outra coisa, é conteúdo extra */
	}
	return true;
}

bool validate_chars(t_bsq *bsq)
{
	for (int i = 0; bsq->grid[i]; ++i)
		for (int j = 0; j < bsq->w; ++j)
			if (bsq->grid[i][j] != bsq->empty && bsq->grid[i][j] != bsq->obstacle)
				return false;
	return true;
}

static void map_error_cleanup(t_bsq *bsq, char *line)
{
	ft_free(bsq->grid);
	free(line);
	fputs("map error\n", stderr);
}

void bsq(FILE *input)
{
	t_bsq  bsq;
	char   *line = NULL;
	size_t cap = 0;

	bsq.grid = NULL;

	if (!read_header(input, &bsq) ||
		!alloc_grid(&bsq) ||
		!read_grid(input, &bsq, &line, &cap) ||
		!check_eof(input, &line, &cap) ||
		!validate_chars(&bsq))
	{
		map_error_cleanup(&bsq, line);
		return;
	}

	/* aqui entra o algoritmo do maior quadrado */

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