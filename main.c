#include<stdlib.h>
#include<stdio.h>

typedef struct cell_t {
  struct cell_t *parent;
  unsigned char data;
} cell_t;

// Min is inclusive, max is not
int randint(int min, int max)
{
  return rand()%(max-min-1) + min;
}

cell_t *get_connectedness(cell_t *cell)
{
  if (!cell) {
    return cell;
  }
  return get_connectedness(cell->parent);
}

void remove_wall(cell_t **maze, int w, int h)
{
  // Randomly pick a wall!
  while (1) {
    int x = randint(0,w);
    int y = randint(0,h);
    int dir = randint(0,4);
    if (!(maze[x][y].data&(1<<dir))) {
      continue;
    }

    // Check to see if they're already in the same congruency class.
    int x2 = x;
    int y2 = y;
    switch (dir) {
    case 0: y2++; break;
    case 1: x2++; break;
    case 2: y2--; break;
    case 3: x2--; break;
    default: break;
    }
    if (get_connectedness(&maze[x][y]) == get_connectedness(&maze[x2][y2])) {
      // They're already congruent, let's pick a different one.
      continue;
    }

    // Remove the wall
    maze[x][y].data = ~((~maze[x][y].data)|(1<<dir));

    // Merge the trees
    cell_t *root1 = get_connectedness(&maze[x][y]);
    cell_t *root2 = get_connectedness(&maze[x2][y2]);
    root1->parent = root2;

    break;
  }
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
  // Generate a maze!
  cell_t **maze = generate_maze(256,256);
  write_file("output.png", maze);
  return 0;
}
