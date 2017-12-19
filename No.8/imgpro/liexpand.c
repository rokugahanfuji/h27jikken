#include <stdio.h>
#include <stdlib.h>
#include "params.h"

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out;

  double amp = 2.0;
  int i, j;
  int th = -1, tl = -1;
  int uh = HIGH, ul = LOW;
  int max, min, w;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'b':
        if (--argc) {
          ul = atoi(*++argv);
          break;
        }
        goto usage;
      case 't':
        if (--argc) {
          uh = atoi(*++argv);
          break;
        }
        goto usage;
      case 'h':
        if (--argc) {
          th = atoi(*++argv);
          break;
        }
        goto usage;
      case 'l':
        if (--argc) {
          tl = atoi(*++argv);
          break;
        }
        goto usage;
      default:
usage:
        fprintf(stderr, "Usage: liexpand [-h high] [-l low] [infile [outfile]]\n");
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

  if (th == -1 || tl == -1) {
    max = LOW;
    min = HIGH;
    for (i = 0; i < image_in.y; i++) {
      for (j = 0; j < image_in.x; j++) {
        w = (int)pixel(image_in, j, i);
        if (w > max) max = w;
        if (w < min) min = w;
      }
    }
  }
  if (th == -1) {
    th = max;
  }
  if (tl == -1) {
    tl = min;
  }
  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      w = (int)((double)(uh - ul) / (double)(th - tl)
         * (double)((int)pixel(image_in, j, i) - tl) + ul + 0.5);
      if (w > uh) pixel(image_out, j, i) = uh;
      else if (w < ul) pixel(image_out, j, i) = ul;
      else pixel(image_out, j, i) = (unsigned char)w;
    }
  }
  return (image_write(image_out, foname));
}

