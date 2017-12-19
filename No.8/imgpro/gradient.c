#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "params.h"

int cx_normal[9] = { 0, 0, 0,
                     0, 1,-1,
                     0, 0, 0};
int cy_normal[9] = { 0, 0, 0,
                     0, 1, 0,
                     0,-1, 0};
int cx_roberts[9] = { 0, 0, 0,
                      0, 1, 0,
                      0, 0,-1};
int cy_roberts[9] = { 0, 0, 0,
                      0, 0, 1,
                      0,-1, 0};
int cx_sobel[9] = {-1, 0, 1,
                   -2, 0, 2,
                   -1, 0, 1};
int cy_sobel[9] = {-1,-2,-1,
                    0, 0, 0,
                    1, 2, 1};

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out;
  double amp = 2.0;

  int *cx = cx_normal;
  int *cy = cy_normal;

  int d[9];
  int i, j, k, dat;
  double xx, yy, zz;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'a':
	if (--argc) {
	  amp = atof(*++argv);
	  break;
	}
        goto usage;
      case 'r':
	cx = cx_roberts;
	cy = cy_roberts;
	break;
      case 's':
	cx = cx_sobel;
	cy = cy_sobel;
	break;
      default:
usage:
        fprintf(stderr, "Usage: gradient [-r | -s] [-a amp] [infile [outfile]]\n");
        exit(-1);
      }
    } else {
      if (finame == NULL) {
        finame = *argv;
      } else if (foname == NULL) {
        foname = *argv;
      } else {
        goto usage;
      }
    }
  }

  image_in = image_read(finame);
  if (image_in.x == 0) return (-1);
  image_out = mkimage(image_in.x, image_in.y);
  if (image_out.x == 0) return (-1);

  for (i = 1; i < image_in.y - 1; i++) {
    for (j = 1; j < image_in.x - 1; j++) {
      d[0] = pixel(image_in, j-1, i-1);
      d[1] = pixel(image_in, j  , i-1);
      d[2] = pixel(image_in, j+1, i-1);
      d[3] = pixel(image_in, j-1, i  );
      d[4] = pixel(image_in, j  , i  );
      d[5] = pixel(image_in, j+1, i  );
      d[6] = pixel(image_in, j-1, i+1);
      d[7] = pixel(image_in, j  , i+1);
      d[8] = pixel(image_in, j+1, i+1);
      xx = yy = 0.0;
      for (k = 0; k < 9; k++) {
	xx += cx[k] * d[k];
	yy += cy[k] * d[k];
      }
      zz = amp * sqrt(xx * xx + yy * yy);
      dat = (int)zz;
      if (dat > 255) dat = 255;
      pixel(image_out, j, i) = (unsigned char)(dat);
    }
  }
  return (image_write(image_out, foname));
}

  
