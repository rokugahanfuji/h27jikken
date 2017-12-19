#include <stdio.h>
#include <math.h>
#include "params.h"

#define PIXEL	1.0

main(int argc, char **argv)
{
  char *foname = NULL;
  Image image_out;
  int i, j;
  int xsize = 0, ysize = 0;
  double cx, cy, x, y, d;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      default:
usage:
        fprintf(stderr, "Usage: czp xsize ysize [outfile]\n");
        exit(-1);
      }
    } else {
      if (xsize == 0) {
	xsize = atoi(*argv);
      } else if (ysize == 0) {
	ysize = atoi(*argv);
      } else if (foname == NULL) {
        foname = *argv;
      } else {
        goto usage;
      }
    }
  }
  if (ysize == 0) goto usage;

  image_out = mkimage(xsize, ysize);
  if (image_out.x == 0) return (-1);

  cx = M_PI / (double)xsize;
  cy = M_PI / (double)ysize;
  for (j = 0; j < ysize; j++) {
    for (i = 0; i < xsize; i++) {
      x = i - (xsize / 2);
      y = j - (ysize / 2);
      d = cx * x * x + cy * y * y;
      d = 112.0 * cos(d) + 128.0;
      pixel(image_out, i, j) = (unsigned char)d;
    }
  }
  return(image_write(image_out, foname));
}

