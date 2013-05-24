#include<stdlib.h>
#include<stdio.h>

typedef struct cell_t {
  struct cell_t *parent;
  unsigned char data;
} cell_t;

cell_t *get_connectedness(cell_t *cell)
{
}

void remove_wall(cell_t **maze, int w, int h)
{
}

cell_t **generate_maze(int w, int h)
{
  cell_t **maze = malloc(sizeof(cell_t*)*w);
  int i;
  for (i=0; i<w; i++) {
    maze[i] = malloc(sizeof(cell_t)*h);
    int j;
    for (j=0; j<h; j++) {
      maze[i][j].data = 0xF;
      maze[i][j].parent = NULL;
    }
  }

  // Alright, now let's go!
  while (1) {
    remove_wall(maze, w, h);
  }

  return maze;
}

void write_file(const char *fname, cell_t **maze)
{
}

int main(int argc, char **argv)
{
  return 0;
}
