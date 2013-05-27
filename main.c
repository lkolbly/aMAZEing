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
      //continue;
      // Try the other directions
      int i;
      for (i=0; i<4; i++) {
	dir = (dir+1)%4;
	if (maze[x][y].data&(1<<dir)) {
	  break;
	}
      }
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
    maze[x2][y2].data = ~((~maze[x2][y2].data)|(1<<((dir+2)%4)));

    // Merge the trees
    cell_t *root1 = get_connectedness(&maze[x][y]);
    cell_t *root2 = get_connectedness(&maze[x2][y2]);
    root1->parent = root2;

    break;
  }
}

// Manage lists of removable walls
typedef struct wall_t {
  int x,y;
  int dir;
} wall_t;

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
  //for (i=0; i<w*h; i++) {
    //remove_wall(maze, w, h);
  //}

  // Generate a list of walls that could be removed.
  cell_t *removeable = malloc(sizeof(cell_t)*256);

  return maze;
}

void write_file(const char *fname, cell_t **maze, int maze_w, int maze_h)
{
}

int main(int argc, char **argv)
{
  // Generate a maze!
  cell_t **maze = generate_maze(256,256);
  write_file("output.png", maze, 256, 256);
  return 0;
}
