#include <stdio.h>
#include "params.h"

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out;
  long hist[LEVEL], w[LEVEL];

  int smooth = 0;
  int i, j, k, max, range;
  long n, sum;
  double d;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 's':
	switch (*++*argv) {
	case '+':
	case '1':
	case '\0':
	  smooth = 1;
	  break;
	case '-':
	case '0':
	  smooth = 0;
	  break;
	default:
	  goto usage;
	}
	break;
      default:
usage:
        fprintf(stderr, "Usage: histimage [-s] [infile [outfile]]\n");
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
  image_out = mkimage(HIST_X_SIZE, HIST_Y_SIZE);
  if (image_out.x == 0) return (-1);

  for (n = 0; n < LEVEL; n++) {
    hist[n] = 0;
  }
  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      n = pixel(image_in, j, i);
      hist[n]++;
    }
  }

  if (smooth == 1) {
    for (n = 0; n < LEVEL; n++) {
      w[n] = hist[n];
    }
    for (n = 0; n < LEVEL; n++) {
      sum = 0;
      for (j = -2; j <= 2; j++) {
	i = n + j;
	if (i <    0   ) i =   0;
	if (i > LEVEL-1) i = LEVEL-1;
	sum += w[i];
      }
      hist[n] = (double)sum / 5.0 + 0.5;
    }
  }

  /* raito = 2 */
  range = HIST_Y_SIZE - 5;
  for (i = 0; i < HIST_Y_SIZE; i++) {
    for (j = 0; j < HIST_X_SIZE; j++) {
      pixel(image_out, j, i) = LOW;
    }
  }
  
  max = 0;
  for (i = 0; i < LEVEL/2; i++) {
    n = hist[2*i] + hist[2*i+1];
    if (n > max) max = n;
  }
  for (i = 0; i < LEVEL/2; i++) {
    d = (double)(hist[2*i] + hist[2*i+1]);
    n = d / (double)max * (double)range;
    for (j = 0; j <= n; j++) {
      pixel(image_out, i, range-j) = HIGH;
    }
  }
  for (i = 0; i <= 4; i++) {
    k = (HIST_X_SIZE / 4) * i;
    if (k >= HIST_X_SIZE) {
      k = HIST_X_SIZE - 1;
    }
    for (j = range; j < HIST_Y_SIZE; j++) {
      pixel(image_out, k, j) = HIGH;
    }
  }

  return (image_write(image_out, foname));
}


