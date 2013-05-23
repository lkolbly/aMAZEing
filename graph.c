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

  // Because we're doing this paging system, we need a notion of whether
  // or not this node is loaded. If this is 1, then it's loaded. If not,
  // then it's not loaded. If it's not loaded, then the disk_cache will
  // name the file that this node is saved in.
  int is_loaded;
  char *disk_cache;
} grid_node_t;

int cache_counter = 0;

char *tempFileName(void)
{
  char *s = malloc(1024);
  sprintf(s, "/tmp/maze%06i.cache", cache_counter++);
  return s;
}

char *saveGridNode(grid_node_t *node)
{
  if (!node->disk_cache) {
    // Generate a name for it
    node->disk_cache = tempFileName();
  }
  FILE *f = fopen(node->disk_cache, "w");
  printf("Writing to %s\n", node->disk_cache);
  if (node->is_leaf) {
    fprintf(f, "LEAF");
    int i;
    for (i=0; i<LEAF_WIDTH; i++) {
      fwrite(&node->grid->nodes[i], LEAF_HEIGHT, 1, f);
    }
  } else {
    // Now save some data about us
    fprintf(f, "NODE");
    fwrite(&node->min_x, 4, 1, f);
    fwrite(&node->max_x, 4, 1, f);
    fwrite(&node->min_y, 4, 1, f);
    fwrite(&node->max_y, 4, 1, f);

    // Save the children to their own file
    int i;
    for (i=0; i<4; i++) {
      char *fname = saveGridNode(node->quads[i]);
      int l = strlen(fname);
      fwrite(&l, 4, 1, f);
      fwrite(fname, l, 1, f);
    }
  }
  fclose(f);

  return node->disk_cache;
}

void freeGridNode(grid_node_t *node)
{
  if (!node->is_loaded) {
    return;
  }
  if (node->is_leaf) {
    int i;
    for (i=0; i<LEAF_WIDTH; i++) {
      free(node->grid->nodes[i]);
    }
    free(node->grid->nodes);
    free(node->grid);
  } else {
    int i;
    for (i=0; i<4; i++) {
      freeGridNode(node->quads[i]);
      free(node->quads[i]);
    }
  }
}

void unloadGridNode(grid_node_t *node)
{
  // First, we want to save it.
  saveGridNode(node);

  // Now we want to free its children.
  freeGridNode(node);
  node->is_loaded = 0;
}

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
  root->is_loaded = 1;
  root->disk_cache = NULL;
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
  grid_node_t *root = newGrid(3, 0,0);
  printf("%i %i %i %i\n", root->min_x, root->min_y, root->max_x, root->max_y);
  char *n = gridGetNodePtr(root, 1, 1);
  *n = 0xFF;
  n = gridGetNodePtr(root, 2, 2);
  *n = 0xFF;

  // Unload it
  unloadGridNode(root->quads[0]);

  return 0;
}
