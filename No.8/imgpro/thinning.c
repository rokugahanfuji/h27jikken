#include <stdio.h>
#include <stdlib.h>
#include "params.h"

#define DEL_CAND  100

int connect4(int near[9]);

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image;

  int end, sum, tmp;
  int i, j, k;
  int near[9];

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      default:
usage:
        fprintf(stderr, "Usage: thinning [infile [outfile]]\n");
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

  do {
    end = 1;

    for (i = 0; i < image.y; i++) {
      for (j = 0; j < image.x; j++) {
        near[0] = pixel(image, j + 1, i);
        near[1] = pixel(image, j + 1, i - 1);
        near[2] = pixel(image, j    , i - 1);
        near[3] = pixel(image, j - 1, i - 1);
        near[4] = pixel(image, j - 1, i);
        near[5] = pixel(image, j - 1, i + 1);
        near[6] = pixel(image, j    , i + 1);
        near[7] = pixel(image, j + 1, i + 1);

        if (pixel(image, j, i) != HIGH) continue; /* not zukei gaso */

        if (near[0]  && near[2] && near[4] && near[6]) /* not kyoukai */
          continue;

        for (k = 0, sum = 0; k < 8; k++) {
          if (near[k]) sum++;
        }
        if (sum <= 1) continue; /* is tanten */

        for (k = 0, sum = 0; k < 8; k++) {
          if (near[k] == HIGH) sum++;
        }
        if (sum == 0) continue; /* is korituten */

        if (connect4(near) != 1) continue; /* renketusei hozon */

        sum = 0;
        for (k = 0; k < 8; k++) {
          if (near[k] == DEL_CAND) {
            near[k] = 0;
            if (connect4(near) != 1) { /* yokohaba 2*/
              sum = 1;
              break;
            }
            near[k] = DEL_CAND;
          }
        }
        if (sum) continue;

        pixel(image, j, i) = DEL_CAND;
        end = 0;
      }
    }
    if (end == 0) {
      for (i = 0; i < image.y; i++) {
        for (j = 0; j < image.x; j++) {
          if (pixel(image, j, i) == DEL_CAND) {
            pixel(image, j, i) = 0;
          }
        }
      }
    }
  } while (end == 0);

  return (image_write(image, foname));
}

int connect4(int near[9])
{
  int i, j, sum;

  near[8] = near[0];
  sum = 0;  
  for (i = 0; i < 8; i += 2) {
     if (near[i] && (near[i+1] == 0 || near[i+2] == 0)) sum++;
  }
  return sum;
}
