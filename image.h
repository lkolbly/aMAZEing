#ifndef IMAGE_H
#define IMAGE_H
#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif
#include "vector3d.h"
#define IMAGE_CODEC_UNKNOWN 0
#define IMAGE_CODEC_PNG 1
typedef struct image_t {
unsigned char *data;
int w, h;
} image_t;
image_t *imageLoadFromFile( const char *filename );
int imagePNGWriteToFile( char *filename, image_t *image );
image_t *imageCreateNewBlank( int w, int h );
image_t *imageCopyRegion( image_t *orig, int llx, int lly, int urx, int ury );
image_t *imageFill( image_t *img, vec3 color, double a );
image_t *imageCopyRegionSkip( image_t *orig, int llx, int lly, int urx, int ury,
int skip );
void imageFree( image_t *img );
#ifdef __cplusplus
}
#endif
#endif 
