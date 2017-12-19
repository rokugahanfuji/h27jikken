#include <stdio.h>
#include <stdlib.h>
#include "params.h"

#define BUFF_MAX 2500

struct xyw {
  int x, y, w;
} buf[BUFF_MAX];

Image image_in, image_out;
long int hist[LEVEL];

void calc_hist(void);
void sort(int level);

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  int i, j, iy, jx, sum;
  int delt;
  int low, high, av;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      default:
usage:
        fprintf(stderr, "Usage: histplane [infile [outfile]]\n");
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

  calc_hist();

  av = (int)(image_in.y * image_in.x / LEVEL);
  high = HIGH;
  low = HIGH;
  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      pixel(image_out, j, i) = 0;
    }
  }

  for (i = HIGH; i > 0; i--) {
    for (sum = 0; sum < av; low --){
      sum = sum + hist[low];
    }
    low++;
    delt = hist[low] - (sum - av);
    sort(low);
    if (low < high) {
      for (iy = 0; iy < image_in.y; iy++){
        for (jx = 0; jx < image_in.x; jx++){
          if (((int)pixel(image_in, jx, iy) >= low + 1) &&
              ((int)pixel(image_in, jx, iy) <= high)) {
            pixel(image_out, jx, iy) = (unsigned char)i;
          }
        }
      }
    }
    for (j = 0; j < delt; j++) {
      pixel(image_out, buf[j].x, buf[j].y) = (unsigned char)i;
      pixel(image_in, buf[j].x, buf[j].y) = (unsigned char)(low + 1);
    }
    hist[low] = hist[low] - delt;
    high = low;
  }
  return (image_write(image_out, foname));
}

void calc_hist()
{
  int i, j, n;

  for (n = 0; n < LEVEL; n++) {
    hist[n] = 0;
  }
  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      n = pixel(image_in, j, i);
      hist[n]++;
    }
  }
}

void sort(int level)
{
  int i, j, inum, wt;
  struct xyw temp;

  int weight(int i, int j);

  inum = 0;
  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      if ((int)pixel(image_in, j, i) == level) {
        wt = weight(i, j);
        buf[inum].y = i;
        buf[inum].x = j;
        buf[inum].w = wt;
        inum++;
      }
    }
  }

  for (i = 0; i < inum - 1; i++) {
    for (j = i + 1; j < inum; j++) {
      if (buf[i].w <= buf[j].w) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
}

int weight(int i, int j)
{
  int dim, djm;
  int dip, djp;
  int k, d[8];
  int r;

  dim = i - 1;
  djm = j - 1;
  dip = i + 1;
  djp = j + 1;
  if (dim < 0) dim = i;
  if (djm < 0) djm = j;
  if (dip > image_in.y - 1) dip = i;
  if (djp > image_in.x - 1) djp = j;

  d[0] = (int)pixel(image_in, djm, dim);
  d[1] = (int)pixel(image_in, j, dim);
  d[2] = (int)pixel(image_in, djp, dim);
  d[3] = (int)pixel(image_in, djm, i);
  d[4] = (int)pixel(image_in, j, i);
  d[5] = (int)pixel(image_in, djp, i);
  d[6] = (int)pixel(image_in, djm, dip);
  d[7] = (int)pixel(image_in, j, dip);
  d[8] = (int)pixel(image_in, djp, dip);
  r = 0;
  for (k = 0; k < 8; k++) {
    r += d[k];
  }
  return(r);
}
