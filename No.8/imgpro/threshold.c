#include <stdio.h>
#include <stdlib.h>
#include "params.h"

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image;

  int thresh = 128;
  int mode = 1;
  int i, j, hi, lo;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 't':
        if (--argc) {
          thresh = atoi(*++argv);
          break;
        }
	goto usage;
      case 'r':
	mode = 2;
        break;
      default:
usage:
        fprintf(stderr, "Usage: threshold [-t n] [-r] [infile [outfile]]\n");
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

  image = image_read(finame);
  if (image.x == 0) return (-1);

  for (i = 0; i < image.y; i++) {
    for (j = 0; j < image.x; j++) {
      if (mode == 2) {
        if ((int)pixel(image, j, i) <= thresh) {
          pixel(image, j, i) = HIGH;
        } else {
          pixel(image, j, i) =  LOW;
        }
      } else {
        if ((int)pixel(image, j, i) >= thresh) {
          pixel(image, j, i) = HIGH;
        } else {
          pixel(image, j, i) =  LOW;
        }
      }
    }
  }
  return (image_write(image, foname));
}

