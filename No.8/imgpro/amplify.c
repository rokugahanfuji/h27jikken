#include <stdio.h>
#include <stdlib.h>
#include "params.h"

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out;

  double amp = 2.0;
  int i, j;
  int w;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'n':
        if (--argc) {
          amp = atof(*++argv);
          break;
        }
        goto usage;
      default:
usage:
        fprintf(stderr, "Usage: amplify [-n n] [infile [outfile]]\n");
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
    for (j = 0; j < image_in.x; j++) {
      w = (int)((double)pixel(image_in, j, i) * amp + 0.5);
      if (w > HIGH) w = HIGH;
      pixel(image_out, j , i) = (unsigned char)w;
    }
  }
  return (image_write(image_out, foname));
}

