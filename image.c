#include<stdio.h>
#include<stdlib.h>
#include<png.h>
#include "image.h" 


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


int main(int argc, char **argv)
{
  image_t *img = malloc(sizeof(image_t));
  img->w = 1028;
  img->h = 1028;
  img->data = malloc(sizeof(char)*img->w*img->h*4);
  int i;
  int j;
  for (i=0; i<img->w; i++)
    {
      for(j=0; j<img->h; j++)
	{
	  img->data[4*img->w*j + 4*i + 0] = rand()%255;
	  img->data[4*img->w*j + 4*i + 1] = rand()%255;
	  img->data[4*img->w*j + 4*i + 2] = rand()%255;
	  img->data[4*img->w*j + 4*i + 3] = 255;
	}
    }
  imagePNGWriteToFile("hi.png",img);
}
