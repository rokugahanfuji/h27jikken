#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "params.h"
#include "lpf.h"

void sub_sample(Image iamge_in, Image image_out, int h, int v);

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out;

  int h = 1;
  int v = 1;
  int ctl = 0;
  int i, j, ii, jj;
  double fx, fy;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'h':
        if (--argc) {
          h = atoi(*++argv);
          break;
        }
	goto usage;
      case 'v':
        if (--argc) {
          v = atoi(*++argv);
          break;
        }
	goto usage;
      case 'f':
        if (--argc) {
          ctl = atoi(*++argv);
          if (ctl >= 0 && ctl <= 2) {
            break;
          }
        }
	goto usage;
      default:
usage:
        fprintf(stderr, "Usage: sample [-h n] [-v n] [-f 0-2] [infile [outfile]]\n");
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

  image_in =image_read(finame);
  if (image_in.x == 0) return (-1);
  image_out = mkimage(image_in.x, image_in.y);
  if (image_out.x == 0) return (-1);

  switch(ctl) {
  case 0:
    sub_sample(image_in, image_out, h, v);
    break;
  case 1:
    fx = 1.0 / (double)h;
    fy = 1.0 / (double)v;
    lp_filter(image_in, image_out, fx, fy);
    sub_sample(image_out, image_out, h, v);
    break;
  case 2:
    fx = 1.0 / (double)h;
    fy = 1.0 / (double)v;
    lp_filter(image_in, image_out, fx, fy);
    break;
  }
  image_write(image_out, foname);
}

void sub_sample(Image image_in, Image image_out, int h, int v)
{
  int i, j, ii, jj;

  for (i = 0; i < image_out.y; i++) {
    for (j = 0; j < image_out.x; j++) {
      ii = (i / v) * v;
      jj = (j / h) * h;
      pixel(image_out, j, i) =  pixel(image_in, jj, ii);
    }
  }
}

