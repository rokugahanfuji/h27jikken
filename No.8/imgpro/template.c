#include <stdio.h>
#include <stdlib.h>
#include "params.h"

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out;
  double amp = 2.0;

  int d0,d1,d2,d3,d4,d5,d6,d7,d8;
  int i,j,k,max,dat;
  int m[8];
  double zz;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'a':
	if (--argc) {
	  amp = atof(*++argv);
	  break;
	}
        goto usage;
      default:
usage:
        fprintf(stderr, "Usage: template [-a amp] [infile [outfile]]\n");
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
      d0 = pixel(image_in, j-1, i-1);
      d1 = pixel(image_in, j  , i-1);
      d2 = pixel(image_in, j+1, i-1);
      d3 = pixel(image_in, j-1, i  );
      d4 = pixel(image_in, j  , i  );
      d5 = pixel(image_in, j+1, i  );
      d6 = pixel(image_in, j-1, i+1);
      d7 = pixel(image_in, j  , i+1);
      d8 = pixel(image_in, j+1, i+1);
      m[0] =  d0+d1+d2+d3-2*d4+d5-d6-d7-d8;
      m[1] =  d0+d1+d2+d3-2*d4-d5+d6-d7-d8;
      m[2] =  d0+d1-d2+d3-2*d4-d5+d6+d7-d8;
      m[3] =  d0-d1-d2+d3-2*d4-d5+d6+d7+d8;
      m[4] = -d0-d1-d2+d3-2*d4+d5+d6+d7+d8;
      m[5] = -d0-d1+d2-d3-2*d4+d5+d6+d7+d8;
      m[6] = -d0+d1+d2-d3-2*d4+d5-d6+d7+d8;
      m[7] =  d0+d1+d2-d3-2*d4+d5-d6-d7+d8;
      max = 0;
      for (k = 0; k < 8; k++) {
        if (max < m[k]) max = m[k];
      }
      zz = amp * (double)max;
      dat = (int)zz;
      if (dat > 255) dat = 255;
      pixel(image_out, j, i) = (unsigned char)dat;
    }
  }
  return (image_write(image_out, foname));
}
