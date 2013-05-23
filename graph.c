#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define LEAF_WIDTH 128
#define LEAF_HEIGHT 128

typedef struct grid_leaf_t {
  char **nodes;
} grid_leaf_t;

typedef struct grid_node_t {
  struct grid_node_t *quads[4];
  // - OR -
  grid_leaf_t *grid;

  int is_leaf;
  int min_x, min_y, max_x, max_y;
} grid_node_t;

grid_leaf_t *newGridLeaf(void)
{
  grid_leaf_t *leaf = malloc(sizeof(grid_leaf_t));
  leaf->nodes = malloc(sizeof(char*)*LEAF_WIDTH);
  int i;
  for (i=0; i<LEAF_WIDTH; i++) {
    leaf->nodes[i] = malloc(LEAF_HEIGHT);
    memset(leaf->nodes[i], 0x0F, LEAF_HEIGHT);
  }
  return leaf;
}

grid_node_t *newGrid(int depth, int min_x, int min_y)
{
  grid_node_t *root = malloc(sizeof(grid_node_t));
  if (depth == 0) {
    root->is_leaf = 1;
    root->grid = newGridLeaf();
    return root;
  }
  int i;
  for (i=0; i<4; i++) {
    int x,y;
    int w = LEAF_WIDTH * powl(2,depth-1);
    int h = LEAF_HEIGHT * powl(2,depth-1);
    switch (i) {
    case 0: x=min_x; y=min_y; break;
    case 1: x=min_x+w; y=min_y; break;
    case 2: x=min_x+w; y=min_y+h; break;
    case 3: x=min_x; y=min_y+h; break;
    default: break;
    };
    root->quads[i] = newGrid(depth-1, x,y);
    root->quads[i]->min_x = x;
    root->quads[i]->min_y = y;
    root->quads[i]->max_x = x+w;
    root->quads[i]->max_y = y+h;
  }
  root->min_x = min_x;
  root->min_y = min_y;
  root->max_x = LEAF_WIDTH * powl(2, depth);
  root->max_y = LEAF_HEIGHT * powl(2,depth);
  return root;
}

char *gridGetNodePtr(grid_node_t *root, int x, int y)
{
  while (1) {
    if (root->is_leaf) {
      return &root->grid->nodes[x-root->min_x][y-root->min_y];
    } else {
      int midx = (root->max_x-root->min_x)/2 + root->min_x;
      int midy = (root->max_y-root->min_y)/2 + root->min_y;
      if (x < midx && y < midy)
	root = root->quads[0];
      else if (x > midx && y < midy)
	root = root->quads[1];
      else if (x > midx && y > midy)
	root = root->quads[2];
      else if (x < midx && y > midy)
	root = root->quads[3];
    }
  }
  return NULL;
}

int main(int argc, char **argv)
{
  grid_node_t *root = newGrid(2, 0,0);
  printf("%i %i %i %i\n", root->min_x, root->min_y, root->max_x, root->max_y);
  char *n = gridGetNodePtr(root, 1, 1);
  *n = 0xFF;
  n = gridGetNodePtr(root, 2, 2);
  *n = 0xFF;

  return 0;
}
