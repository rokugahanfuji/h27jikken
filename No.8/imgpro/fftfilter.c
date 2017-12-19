#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "params.h"
/* #include "fft.h" */

void fft2(double a_rl[], double a_im[], double b_rl[], double b_im[], int exh, int exv, int inv, double *sin_tbl, double *cos_tbl);
void adjust(Image image_in, Image image_wx);

double *ar, *ai, *br, *bi;
double *cos_tbl, *sin_tbl;

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out, image_wk;

  double norm, max, min;
  int i, j, k, w, n, exh, exv;
  double a, b, circ, zx, zy;
  int ratio = 0;

  n = 1;
  a = 0.0;
  b = -1.0;
  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'a':
        if (--argc) {
          a = atof(*++argv);
          break;
        }
	goto usage;
      case 'b':
        if (--argc) {
          b = atof(*++argv);
          break;
        }
	goto usage;
      case 'r':
	ratio = 1;
	break;
      case 'n':
        n = 0;
        break;
      default:
usage:
        fprintf(stderr, "Usage: fftfilter [-r] [-a n] [-b n] [infile [outfile]]\n");
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
  circ = sqrt(((double)image_in.x / 2.0) * ((double)image_in.x / 2.0)
            + ((double)image_in.y / 2.0) * ((double)image_in.y / 2.0));
  if (ratio) {
    a *= circ;
    b *= circ;
  }
  if (b < 0) {
    b = circ + 1;
  }
  
  if (image_in.x == 0) return (-1);
  for (i = 1, exh = 0; i < image_in.x; i *= 2, exh++);
  for (j = 1, exv = 0; j < image_in.y; j *= 2, exv++);
  if (image_in.x == i && image_in.y == j) {
    image_wk = image_in;
    zx = zy = 1.0;
  } else {
    image_wk = mkimage(i, j);
    if (image_wk.x == 0) return (-1);
    adjust(image_in, image_wk);
    zx = (double)image_wk.x / (double)image_in.x;
    zy = (double)image_wk.y / (double)image_in.y;
  }
  
  image_out = mkimage(i, j);
  if (image_out.x == 0) return (-1);

  ar = malloc(sizeof(double) * i * j); /* ar[Ysize][Xsize] */
  ai = malloc(sizeof(double) * i * j); /* ai[Ysize][Xsize] */
  br = malloc(sizeof(double) * i * j); /* br[X_SIZE][Y_SIZE] */
  bi = malloc(sizeof(double) * i * j); /* bi[X_SIZE][Y_SIZE] */
  k = (i > j) ? i : j;
  cos_tbl = malloc(sizeof(double) * k);
  sin_tbl = malloc(sizeof(double) * k);
  if (ar == NULL || ai == NULL ||  cos_tbl == NULL || sin_tbl == NULL) {
    return(-1);
  }

  for (i = 0; i < image_wk.y; i++) {
    for (j = 0; j < image_wk.x; j++) {
      ar[i * image_wk.x + j] = (double)pixel(image_wk, j, i);
      ai[i * image_wk.x + j] = 0.0;
    }
  }
  fft2(ar, ai, br, bi, exh, exv, 1, sin_tbl, cos_tbl);
  for (i = 0; i < image_wk.y; i++) {
    for (j = 0; j < image_wk.x; j++) {
      norm = ((double)i-(double)image_wk.x/2.0) / zx * ((double)i-(double)image_wk.x/2.0) / zx
           + ((double)j-(double)image_wk.y/2.0) / zy * ((double)j-(double)image_wk.y/2.0) / zy;
      circ = sqrt(norm);
      if ((circ < a) || (b < circ)) {
        ar[i * image_wk.x + j] = 0.0;
	ai[i * image_wk.x + j] = 0.0;
      }
    }
  }
  fft2(ar, ai, br, bi, exh, exv, -1, sin_tbl, cos_tbl);
  max = min = 0;
  for (i = 0; i < image_wk.y; i++) {
    for (j = 0; j < image_wk.x; j++) {
      norm = ar[i * image_wk.x + j];
      if (norm > max) max = norm;
      if (norm < min) min = norm;
    }
  } 
  for (i = 0; i < image_out.y; i++) {
    for (j = 0; j < image_out.x; j++) {
      if (n == 0) {
        w = (int)(ar[i * image_out.x + j] * HIGH / max);
        if (w < 0) w = 0;
      } else {
        w = (int)((ar[i * image_out.x + j] - min) * HIGH / (max - min));
      }
      pixel(image_out, j, i) = w;
    }
  } 
 
  if (image_out.x == image_in.x && image_out.y == image_in.y) {
    image_write(image_out, foname);
  } else {
    adjust(image_out, image_in);
    image_write(image_in, foname);
  }
}

