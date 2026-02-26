#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct s_gl
{
    int w;
    int h;
    int it;
    char** grid;
}       t_gl;

void ft_free(char** grid)
{
    if(!grid)
        return;
    int y = 0;
    while(grid[y])
        free(grid[y++]);
    free(grid);
}

char** alloc_grid(int w, int h)
{
    char** grid = malloc(sizeof(char*) * (h + 1));
    if (!grid)
        return NULL;
    for (int y = 0; y <= h; ++y)
        grid[y] = NULL;
    for (int y = 0; y < h; ++y)
    {
        grid[y] = malloc(sizeof(char) * (w + 1));
        if (!grid[y])
        {
            ft_free(grid);
            return NULL;
        }
        for (int x = 0; x < w; ++x)
            grid[y][x] = ' ';
        grid[y][w] = '\0';
    }
    return grid;
}

bool init_game(t_gl* g, char** argv)
{
    g->w = atoi(argv[1]);
    g->h = atoi(argv[2]);
    g->it = atoi(argv[3]);
    if (g->w <= 0 || g->h <= 0 || g->it < 0)
        return false;
    g->grid = alloc_grid(g->w, g->h);
    if (!g->grid)
        return false;
    return true;
}

void apply_sketch(t_gl* g)
{
    char c;
    int x = 0;
    int y = 0;
    bool draw = false;
    while (read(0, &c, 1) == 1)
    {
        switch (c)
        {
            case 'w': if (y > 0) --y; break;
            case 'a': if (x > 0) --x; break;
            case 's': if (y < g->h - 1) ++y; break;
            case 'd': if (x < g->w - 1) ++x; break;
            case 'x': draw = !draw;
        }
        if (draw)
            g->grid[y][x] = 'O';
    }
}

int count_neighbours(t_gl* g, int x, int y)
{
    int n = 0;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (!dx && !dy)
                continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx < 0 || ny < 0 || nx >= g->w || ny >= g->h)
                continue;
            if (g->grid[ny][nx] == 'O')
                ++n;
        }
    }
    return n;
}

bool sim_life(t_gl* g)
{
    for (int i = 0; i < g->it; ++i)
    {
        char** new_day = alloc_grid(g->w, g->h);
        if (!new_day)
            return false;
        for (int y = 0; y < g->h; ++y)
        {
            for (int x = 0; x < g->w; ++x)
            {
                int n = count_neighbours(g, x, y);
                if (g->grid[y][x] == 'O')
                    new_day[y][x] = (n == 2 || n == 3) ? 'O' : ' ';
                else
                    new_day[y][x] = (n == 3) ? 'O' : ' ';
            }
        }
        ft_free(g->grid);
        g->grid = new_day;
    }
    return true;
}

void print_grid(t_gl* g)
{
    for (int y = 0; y < g->h; ++y)
    {
        for (int x = 0; x < g->w; ++x)
            putchar(g->grid[y][x]);
        putchar('\n');
    }
}

int main(int argc, char** argv)
{
    t_gl g = {0};
    if (argc != 4 || !init_game(&g, argv))
        return 1;
    apply_sketch(&g);
    if (!sim_life(&g))
        return 2;
    print_grid(&g);
    ft_free(g.grid);
    return 0;
}