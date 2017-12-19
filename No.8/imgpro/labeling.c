#include <stdio.h>
#include <stdlib.h>
#include "params.h"

#define L_BASE	100

void labelset(Image image, int xy, int ys, int label);

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out;

  int i, j;
  int label;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      default:
usage:
        fprintf(stderr, "Usage: labeling [infile [outfile]]\n");
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
      pixel(image_out, j, i) = pixel(image_in, j, i);
    }
  }
  label = L_BASE;
  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      if (pixel(image_out, j, i) == HIGH) {
        if (label >= HIGH) {
          fprintf(stderr, "ERROR! too many labels. \n");
          exit(1);
        }
        labelset(image_out, j, i, label);
        label++;
      }
    }
  }
  fprintf(stderr, "no. of labels: %d\n", label - L_BASE);
  return (image_write(image_out, foname));
}

void labelset(Image image, int xs, int ys, int label)
{
  int i, j, cnt;

  pixel(image, xs, ys) = label;
  for (;;) {
    cnt = 0;
    for (i = 0; i < image.y; i++) {
      for (j = 0; j < image.x; j++) {
        if (pixel(image, j, i) == label) {
          if (pixel(image, j+1, i  ) == HIGH){
	    pixel(image, j+1, i  ) = label; cnt++;
	  }
          if (pixel(image, j+1, i-1) == HIGH){
	    pixel(image, j+1, i-1) = label; cnt++;
	  }
          if (pixel(image, j  , i-1) == HIGH){
	    pixel(image, j  , i-1) = label; cnt++;
	  }
          if (pixel(image, j-1, i-1) == HIGH){
	    pixel(image, j-1, i-1) = label; cnt++;
	  }
          if (pixel(image, j-1, i  ) == HIGH){
	    pixel(image, j-1, i  ) = label; cnt++;
	  }
          if (pixel(image, j-1, i+1) == HIGH){
	    pixel(image, j-1, i+1) = label; cnt++;
	  }
          if (pixel(image, j  , i+1) == HIGH){
	    pixel(image, j  , i+1) = label; cnt++;
	  }
          if (pixel(image, j+1, i+1) == HIGH){
	    pixel(image, j+1, i+1) = label; cnt++;
	  }
        }
      }
    }
    if (cnt == 0) return;
  }
}

