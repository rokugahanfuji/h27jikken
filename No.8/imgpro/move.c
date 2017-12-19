#include <stdio.h>
#include <stdlib.h>
#include "params.h"

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out;

  int i, j, m, n, d;
  int xs, ys;
  double px = 0.0, py = 0.0;
  double x, y, p, q;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'p':
        switch (*++*argv) {
        case 'x':
          if (--argc) {
            px = atof(*++argv);
            break;
          }
	  goto usage;
        case 'y':
          if (--argc) {
            py = atof(*++argv);
            break;
          }
	  goto usage;
        default:
          goto usage;
        }
        break;
      default:
usage:
        fprintf(stderr, "Usage: move [-px x_move] [-py y_move] [infile [outfile]]\n");
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
  xs = image_in.x / 2;
  ys = image_in.y / 2;
  image_out = mkimage(image_in.x, image_in.y);
  if (image_out.x == 0) return (-1);
  
  for (i = -ys; i < ys; i++) {
    for (j = -xs; j < xs; j++) {
      y = i - py;
      x = j - px;
      if (y > 0) {
        m = y;
      } else {
        m = y - 1;
      }
      if (x > 0) {
        n = x;
      } else {
        n = x - 1;
      }
      q = y - m;
      p = x - n;
      if ((m >= -ys) && (m < ys) && (n >= -xs) && (n <xs)) {
        d = (1.0 - q)*((1.0 - p) * pixel(image_in, n+xs, m+ys)
                            + p  * pixel(image_in, n+1+xs, m+ys))
                 + q *((1.0 - p) * pixel(image_in, n+xs, m+1+ys)
                            + p  * pixel(image_in, n+1+xs, m+1+ys));
      } else {
        d = 0;
      }
      if (d < LOW) d = LOW;
      if (d > HIGH) d = HIGH;
      pixel(image_out, j+xs, i+ys) = d;
    }
  }
  return (image_write(image_out, foname));
}

