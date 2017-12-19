#include <stdio.h>
#include <stdlib.h>
#include "params.h"

#define BIT 8

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in;

  int bit = 8;
  int i, j;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'b':
        if (--argc) {
          bit = atoi(*++argv);
          if (bit >= 1 && bit <= 8) {
            break;
          }
        }
	goto usage;
      default:
usage:
        fprintf(stderr, "Usage: quantize [-b 0-8] [infile [outfile]]\n");
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
  
  bit = BIT - bit;
  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      pixel(image_in, j, i) = (pixel(image_in, j, i) >> bit) << bit;
    }
  }
  image_write(image_in, foname);
}

