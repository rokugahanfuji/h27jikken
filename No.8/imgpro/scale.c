#include <stdio.h>
#include <stdlib.h>
#include "params.h"

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out;

  int mode = 1, resize = 0;
  int i, j, m, n, d;
  int xs, ys, xs_in, ys_in;
  double zx = 1.0, zy = 1.0;
  double x, y, p, q;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'z':
        switch (*++*argv) {
        case 'x':
          if (--argc) {
            zx = atof(*++argv);
            break;
          }
	  goto usage;
        case 'y':
          if (--argc) {
            zy = atof(*++argv);
            break;
          }
	  goto usage;
        default:
          goto usage;
        }
        break;
      case 'n':
	mode = 2;
        break;
      case 's':
	resize = 1;
	break;
      default:
usage:
        fprintf(stderr, "Usage: scale [-s] [-n] [-zx x_scale] [-zy y_scale] [infile [outfile]]\n");
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
  xs_in = image_in.x / 2;
  ys_in = image_in.y / 2;

  if (resize == 0) {
    image_out = mkimage(image_in.x, image_in.y);
  } else {
    image_out = mkimage((int)(image_in.x * zx), (int)(image_in.y * zy));
  }
  if (image_out.x == 0) return (-1);
  xs = image_out.x / 2;
  ys = image_out.y / 2;

  if (mode == 2) {
    for (i = -ys; i < ys; i++) {
      for (j = -xs; j < xs; j++) {
        if (i > 0) {
          m = i / zy + 0.5;
        } else {
          m = i / zy - 0.5;
        }
        if (j > 0) {
          n = j / zx + 0.5;
        } else {
          n = j / zx - 0.5;
        }
        if ((m >= -ys_in) && (m < ys_in) && (n >= -xs_in) && (n < xs_in)) {
          pixel(image_out, j+xs, i+ys) = pixel(image_in, n+xs_in, m+ys_in);
        } else {
          pixel(image_out, j+xs, i+ys) = 0;
        }
      }
    }
  } else {
    for (i = -ys; i < ys; i++) {
      for (j = -xs; j < xs; j++) {
        y = i / zy;
        x = j / zx;
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
        if ((m >= -ys_in) && (m < ys_in) && (n >= -xs_in) && (n <xs_in)) {
          d = (1.0 - q)*((1.0 - p) * pixel(image_in, n+xs_in, m+ys_in)
                              + p  * pixel(image_in, n+1+xs_in, m+ys_in))
                   + q *((1.0 - p) * pixel(image_in, n+xs_in, m+1+ys_in)
                              + p  * pixel(image_in, n+1+xs_in, m+1+ys_in));
        } else {
          d = 0;
        }
        if (d < LOW) d = LOW;
        if (d > HIGH) d = HIGH;
        pixel(image_out, j+xs, i+ys) = d;
      }
    }
  }
  return (image_write(image_out, foname));
}

