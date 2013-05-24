typedef struct image_t {
unsigned char *data;
int w, h;
} image_t;
image_t *imageLoadFromFile( const char *filename );
int imagePNGWriteToFile( char *filename, image_t *image );
image_t *imageCreateNewBlank( int w, int h );
image_t *imageCopyRegion( image_t *orig, int llx, int lly, int urx, int ury );
image_t *imageCopyRegionSkip( image_t *orig, int llx, int lly, int urx, int ury,
int skip );
void imageFree( image_t *img );
