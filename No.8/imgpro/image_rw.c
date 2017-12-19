#include <stdio.h>
#include <stdlib.h>
#include "params.h"

#define BUFSIZ	1024

Image mkimage(int xsiz, int ysiz)
{
  Image image;

  image.x = xsiz;
  image.y = ysiz;
  image.img = (unsigned char *)malloc(sizeof(unsigned char) * xsiz * ysiz);
  if (image.img == NULL) {
    fprintf(stderr, "mkimage: out of memory.\n");
  }
  return (image);
}

Image image_read(char *fname)
{
  FILE *fp;
  int  i, j, c;
  int  xsiz, ysiz, max;
  char buf[BUFSIZ];
  Image image;

  if (fname != NULL) {
    if ((fp = fopen(fname, "r")) == NULL) {
      fprintf(stderr, "image_read: can't open.\n");
      image.x = EOF;
      return(image);
    }
  } else {
    fp = stdin;
  }

  if (fgets(buf, BUFSIZ, fp) == NULL) {
    fprintf(stderr, "image_read: no data.\n");
    if (fname != NULL) fclose(fp);
    image.x = EOF;
    return(image);
  }
 
  if (!(buf[0] == 'P' && (buf[1] == '2' || buf[1] == '5'))) {
    fprintf(stderr, "image_read: This isn't a pgm file.\n");
    if (fname != NULL) fclose(fp);
    image.x = 0;
    image.y = 2;
    return (image);
  }
  if (buf[1] == '2') {
    fprintf(stderr, "image_read: ascii encoding isn't suported.\n");
    if (fname != NULL) fclose(fp);
    image.x = 0;
    image.y = 3;
    return (image);
  }

  do {
    if (fgets(buf, BUFSIZ, fp) == NULL) {
      goto BADHEADER;
    }
  } while (buf[0] == '#');

  if (sscanf(buf, "%d %d", &xsiz, &ysiz) != 2) {
    goto BADHEADER;
  }
  if (xsiz <= 0 || ysiz <= 0) {
    fprintf(stderr, "image_read: size (%d,%d) isn't supported.\n", xsiz, ysiz);
    if (fname != NULL) fclose(fp);
    image.x = 0;
    image.y = 4;
    return (image);
  }
    
  if (fgets(buf, BUFSIZ, fp) == NULL) {
    goto BADHEADER;
  }
  if (sscanf(buf, "%d", &max) != 1) {
BADHEADER:
    fprintf(stderr, "image_read: bad header.\n");
    if (fname != NULL) fclose(fp);
    image.x = 0;
    image.y = 5;
    return (image);
  }
  
  image = mkimage(xsiz, ysiz);
  if (image.img == NULL) {
    if (fname != NULL) fclose(fp);
    image.x = 0;
    image.y = 6;
    return (image);
  }

  for (i = 0; i < ysiz; i++) {
    for (j = 0; j < xsiz; j++) {
      if ((c = fgetc(fp)) == EOF) {
        fprintf(stderr, "image_read: short of data.\n");
        if (fname != NULL) fclose(fp);
	free(image.img);
	image.x = 0;
	image.y = 6;
	return (image);
      } 
      pixel(image, j, i) = c;
    }
  }
  if (fname != NULL) fclose(fp);
  return(image);
}

int image_write(Image image, char *fname)
{
  FILE *fp;
  int i, j;

  if (fname != NULL) {
    if ((fp = fopen(fname, "w")) == NULL) {
      fprintf(stderr, "image_write: can't open.\n");
      return(EOF);
    }
  } else {
    fp = stdout;
  }

  fprintf(fp, "P5\n%d %d\n255\n", image.x, image.y);
  for (i = 0; i < image.y; i++) {
    for (j = 0; j < image.x; j++) {
      fputc((int)pixel(image, j, i), fp);
    }
  }
  if (fname != NULL) fclose(fp);
  return (0);
}

