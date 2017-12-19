#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "params.h"
/* #include "fft.h" */

void fft2(double a_rl[], double a_im[], double b_rl[], double b_im[], int exh, int exv, int inv, double *sin_tbl, double *cos_tbl);
void adjust(Image image_in, Image image_wx);

double *ar, *ai, *ff, *br, *bi;
double *cos_tbl, *sin_tbl;

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out, image_wk;
  double norm, max, min;
  int i, j, k, w, n, exh, exv;

  n = 1;
  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'n':
        n = 0;
        break;
      default:
usage:
        fprintf(stderr, "Usage: fftimage [infile [outfile]]\n");
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
  for (i = 1, exh = 0; i < image_in.x; i *= 2, exh++);
  for (j = 1, exv = 0; j < image_in.y; j *= 2, exv++);
  if (image_in.x == i && image_in.y == j) {
    image_wk = image_in;
  } else {
    image_wk = mkimage(i, j);
    if (image_wk.x == 0) return (-1);
    adjust(image_in, image_wk);
  }
  
  image_out = mkimage(i, j);
  if (image_out.x == 0) return (-1);

  ar = malloc(sizeof(double) * i * j); /* ar[Ysize][Xsize] */
  ai = malloc(sizeof(double) * i * j); /* ai[Ysize][Xsize] */
  br = malloc(sizeof(double) * i * j); /* br[X_SIZE][Y_SIZE] */
  bi = malloc(sizeof(double) * i * j); /* bi[X_SIZE][Y_SIZE] */
  ff = malloc(sizeof(double) * i * j); /* ff[Ysize][Xsize] */
  k = (i > j) ? i : j;
  cos_tbl = malloc(sizeof(double) * k);
  sin_tbl = malloc(sizeof(double) * k);
  if (ar == NULL || ai == NULL || ff == NULL || cos_tbl == NULL || sin_tbl == NULL) {
    return(-1);
  }

  for (i = 0; i < image_wk.y; i++) {
    for (j = 0; j < image_wk.x; j++) {
      ar[i * image_wk.x + j] = (double)pixel(image_in, j, i);
      ai[i * image_wk.x + j] = 0.0;
    }
  }
  fft2(ar, ai, br, bi, exh, exv, 1, sin_tbl, cos_tbl);
  max = min = 0;
  for (i = 0; i < image_wk.y; i++) {
    for (j = 0; j < image_wk.x; j++) {
      norm = ar[i * image_wk.x + j] * ar[i * image_wk.x + j]
           + ai[i * image_wk.x + j] * ai[i * image_wk.x + j];
      if (norm != 0.0) {
        norm = log(norm) / 2.0;
      } else {
        norm = 0.0;
      }
      ff[i * image_wk.x + j] = norm;
      if (norm > max) max = norm;
      if (norm < min) min = norm;
    }
  }
  for (i = 0; i < image_out.y; i++) {
    for (j = 0; j < image_out.x; j++) {
      if (n == 0) {
        w = (int)(ff[i * image_out.x + j] * HIGH / max);
        if (w < 0) w = 0;
      } else {
        w = (int)((ff[i * image_out.x + j] - min) * HIGH / (max - min));
      }
      pixel(image_out, j, i) = w;
    }
  }
  if (image_out.x == image_in.x && image_out.y == image_in.y) {
    return (image_write(image_out, foname));
  } else {
    adjust(image_out, image_in);
    return (image_write(image_in, foname));
  }
}
