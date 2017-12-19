#include <stdio.h>
#include <stdlib.h>
#include "params.h"

#define BUFSIZ	1024
#define DEFAULT_MASKFILE "mask.pgm"

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL, *fmname = NULL;
  Image image_in, image_out, image_mask;

  int i, j;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'f':
        if (--argc) {
          fmname = *++argv;
          break;
        }
        goto usage;
      default:
usage:
        fprintf(stderr, "Usage: mask [-f mask_file] < infile > outfile\n");
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
  if (fmname == NULL) fmname = DEFAULT_MASKFILE;

  image_mask = image_read(fmname);
  if (image_mask.x == 0) return (-1);
  image_in = image_read(finame);
  if (image_in.x == 0) return (-1);
  if (image_in.x != image_mask.x || image_in.y != image_mask.y) {
    fprintf(stderr, "Input image and mask image should be same size.\n");
    return(-1);
  }

  image_out = mkimage(image_in.x, image_in.y);
  if (image_out.x == 0) return (-1);

  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      if (pixel(image_mask, j, i) == HIGH) {
        pixel(image_out, j, i) = pixel(image_in, j, i);
      } else {
        pixel(image_out, j, i) = LOW;
      }
    }
  }
  return (image_write(image_out, foname));
}

