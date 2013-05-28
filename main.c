#include<stdlib.h>
#include<stdio.h>

#include<png.h>
#include "image.h"
#include<math.h> 


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
  if (!cell->parent) {
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
  wall_t *removeable = malloc(sizeof(wall_t)*w*h*4);
  int j, cnt=0;
  for (i=1; i<w-1; i++) {
    for (j=1; j<h-1; j++) {
      removeable[cnt].x = i;
      removeable[cnt].y = j;
      removeable[cnt].dir = cnt%4;
      cnt++;
    }
  }

  int nwalls = cnt;
  printf("There are %i removeable walls.\n", nwalls);
  while (nwalls > 0) {
    int pick;
    if (nwalls > 2) {
      pick = randint(0,nwalls-1);
    } else {
      pick = 0;
    }
    int x = removeable[pick].x;
    int y = removeable[pick].y;
    printf("%i out of %i (%i,%i,%i)\n", pick, nwalls, x, y, removeable[pick].dir);

    // Check to see if there's a path between the neighboring points.
    int x2 = x;
    int y2 = y;
    switch (removeable[pick].dir) {
    case 0: y2++; break;
    case 1: x2++; break;
    case 2: y2--; break;
    case 3: x2--; break;
    default: break;
    }

    int dir = removeable[cnt].dir;
    printf("%p %p\n", get_connectedness(&maze[x][y]), get_connectedness(&maze[x2][y2]));
    if (get_connectedness(&maze[x][y]) != get_connectedness(&maze[x2][y2]) || get_connectedness(&maze[x][y]) == NULL) {
      // Remove the wall
      maze[x][y].data = ~((~maze[x][y].data)|(1<<dir));
      maze[x2][y2].data = ~((~maze[x2][y2].data)|(1<<((dir+2)%4)));

      // Merge the trees
      cell_t *root1 = get_connectedness(&maze[x][y]);
      cell_t *root2 = get_connectedness(&maze[x2][y2]);
      root1->parent = root2;
    }

    memmove(&removeable[pick], &removeable[pick+1], sizeof(wall_t)*(nwalls-pick-1));
    nwalls--;
  }

  return maze;
}

void write_file(const char *fname, cell_t **maze, int maze_w, int maze_h)
{
  image_t *img = malloc(sizeof(image_t));
  img->w = maze_w*10+10;
  img->h = maze_h*10+10;
  img->data = malloc(sizeof(char)*img->w*img->h*4);


  int i;
  int j;
  int iMazeMax;
  int jMazeMax;


  i=0;
  iMazeMax=maze_w;
  j=0;
  jMazeMax=maze_h;

  for (i=0; i<iMazeMax*jMazeMax*440;i++)
    {
      img->data[i] = 255;
    }
  

  for (i=0; i<iMazeMax; i++)
    {
      for (j=0; j<jMazeMax; j++)
	{
	  if((maze[i][j].data & (1<<0)) == 1) // && j>1)
	    {
	      int k;
	      // for(k=-5;k<=5;k++)
	      //	{
	      blacken(img,i,j);
		  //	}
	    }
	  if((maze[i][j].data & (1<<1)) == 1)
	    {
	      //blacken(img,i,j);
	      
	    }
	  if((maze[i][j].data & (1<<2)) == 1)
	    {
	      // blacken(img,i,j);
	      
	    }
	  if((maze[i][j].data & (1<<3)) == 1)
	    {
	      
	    }
	}
    }
  imagePNGWriteToFile(fname,img);
}

void blacken( image_t *img, int x, int y)
{
  int k;
  //for(k=-5; k<=5; k++)
  // {
  img->data[x*10*4 + y*10*(img->w)*4 + 0 + 40 + 40*img->w]=0;
  img->data[x*10*4 + y*10*(img->w)*4 + 1 + 40 + 40*img->w]=0;
  img->data[x*10*4 + y*10*(img->w)*4 + 2 + 40 + 40*img->w]=0;
  img->data[x*10*4 + y*10*(img->w)*4 + 3 + 40 + 40*img->w]=255;
  // }
}

int main(int argc, char **argv)
{
  // Generate a maze!
  cell_t **maze = generate_maze(25,25);
  write_file("output.png", maze, 25, 25);
  return 0;
}


int imagePNGWriteToFile( char *filename, image_t *image )
{

  /* Open the file... */
  FILE *f = fopen( filename, "wb" );

  if (!f) {
    return 1;
  };

  /* Start setting up libpng to write... */
  png_structp png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING,
						 NULL, NULL, NULL );

  if (!png_ptr) {
    /* Error? */
  };

  png_infop info_ptr = png_create_info_struct( png_ptr );

  if (!info_ptr) {
    png_destroy_write_struct( &png_ptr, NULL );
    /* Error? */
  };

  /* Init the io... */
  png_init_io( png_ptr, f );

  /* Tell it the write status function we want to use... */
  // png_set_write_status_fn( png_ptr, imagePNGWriteStatusCallback );

  /* Setup the header... */
  png_set_IHDR( png_ptr, info_ptr, image->w, image->h, 8,
		PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );

  /* Setup the pointers to the rows in the image... */
  unsigned char *row_pointers[image->h];
  int i, j=0;

  for (i=0; i<image->h; i++) {
    //row_pointers[i] = malloc(image->w*4);
    row_pointers[image->h-i-1] = &image->data[j];
    j+=4*image->w;
  };

  png_set_rows( png_ptr, info_ptr, row_pointers );

  /* Write out the image... */
  png_write_png( png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL );

  /* Done! Let's clean up. */
  fclose( f );

  return 0;
} 
