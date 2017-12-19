#include <stdio.h>
#include <stdlib.h>
#include "params.h"

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out;
  double amp = 2.0;

  int d[9];
  int i,j,k,max,dat;
  int m[9];
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
      d[0] = pixel(image_in, j-1, i-1);
      d[1] = pixel(image_in, j  , i-1);
      d[2] = pixel(image_in, j+1, i-1);
      d[3] = pixel(image_in, j-1, i  );
      d[4] = pixel(image_in, j  , i  );
      d[5] = pixel(image_in, j+1, i  );
      d[6] = pixel(image_in, j-1, i+1);
      d[7] = pixel(image_in, j  , i+1);
      d[8] = pixel(image_in, j+1, i+1);

      for (int k = 0; k < 9; k++) {
        if (d[k] == 255){
          max = 255;
          break;
        } else {
          max = 0;
        }
      }
        zz = amp * (double)max;
        dat = (int)zz;
        if (dat > 255) dat = 255;
        pixel(image_out, j, i) = (unsigned char)dat;
    }
  }
  return (image_write(image_out, foname));
}
