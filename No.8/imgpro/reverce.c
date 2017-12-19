#include <stdio.h>
#include <stdlib.h>
#include "params.h"

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image;

  int i, j;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      default:
usage:
        fprintf(stderr, "Usage: reverce [infile [outfile]]\n");
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
      pixel(image, j, i) = HIGH - pixel(image, j, i);
    }
  }
  return (image_write(image, foname));
}

