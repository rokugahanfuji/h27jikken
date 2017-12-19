#include <stdio.h>
#include <stdlib.h>
#include "params.h"

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out;
  unsigned char mean(unsigned char c[]);
  unsigned char median(unsigned char c[]);

  int which = 0, kai = 1;
  int i, j, k;
  unsigned char c[9];

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'n':
        if (--argc) {
          kai = atoi(*++argv);
          break;
        }
	goto usage;
      case 'i':
	which = 1;
        break;
      default:
usage:
        fprintf(stderr, "Usage: smooth [-i] [infile [outfile]]\n");
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

  for (i = 0; i < image_in.y; i++) {
    pixel(image_out, 0, i) = pixel(image_in, 0, i);
    pixel(image_out, image_in.x - 1, i) = pixel(image_in, image_in.x - 1, i);
  }
  for (j = 0; j < image_in.x; j++) {
    pixel(image_out, j, 0) = pixel(image_in, j , 0);
    pixel(image_out, j, image_in.y - 1) = pixel(image_in, j, image_in.y - 1);
  }
  i = image_in.y - 1;
  for (j = 0; j < image_in.x ; j++) {
    pixel(image_out, j, 0) = pixel(image_in, j, 0);
    pixel(image_out, j, i) = pixel(image_in, j, i);
  }
  j = image_in.x - 1;
  for (i = 1; i < image_in.y - 1; i++) {
    pixel(image_out, 0, i) = pixel(image_in, 0, i);
    pixel(image_out, j, i) = pixel(image_in, j, i);
  }
  for (k = 0; k < kai; k++) {
    for (i = 1; i < image_in.y - 1; i++) {
      for (j = 1; j < image_in.x - 1; j++) {
        c[0] = pixel(image_in, j-1, i-1);
        c[1] = pixel(image_in, j  , i-1);
        c[2] = pixel(image_in, j+1, i-1);
        c[3] = pixel(image_in, j-1, i);
        c[4] = pixel(image_in, j  , i);
        c[5] = pixel(image_in, j+1, i);
        c[6] = pixel(image_in, j-1, i+1);
        c[7] = pixel(image_in, j  , i+1);
        c[8] = pixel(image_in, j+1, i+1);
        if (which == 1) {
  	  pixel(image_out, j, i) = mean(c);
        } else {
	  pixel(image_out, j, i) = median(c);
        }
      }
    }
    for (i = 1; i < image_in.y - 1; i++) {
      for (j = 1; j < image_in.x - 1; j++) {
	pixel(image_in, j, i) = pixel(image_out, j, i);
      }
    } 
  }
  return (image_write(image_out, foname));
}

unsigned char mean(unsigned char c[])
{
  int i, s;

  s = 0;
  for (i = 0; i < 9; i++) {
    s += (int)c[i];
  }
  return((unsigned char)(s / 9));
}

unsigned char median(unsigned char c[])
{
  int i, j;
  unsigned char buf;

  for (i = 1; i < 9; i++) {
    for (j = i; j > 0 && c[j-1] > c[j]; j--) {
      buf = c[j];
      c[j] = c[j-1];
      c[j-1] = buf;
    }
  }
  return (c[4]);
}
