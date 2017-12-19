#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "params.h"

#define L_BASE	100

Image image_in, image_out;

double calc_size(int label, int *cx, int *cy);
double calc_length(int label);

main(int argc, char **argv)
{
  int xh = -1, xl = -1, yh = -1, yl = -1;
  double sh = -1.0, sl = -1.0, rh = -1.0, rl = -1.0, lh = -1.0, ll = -1.0;

  int i, j;
  int cnt, w;
  double *size, *ratio, *length;
  int *gx, *gy, *lno;

  int x, y; 
  char *finame = NULL, *foname = NULL;

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 's':
        switch (*++*argv) {
        case 'h':
          if (--argc) {
            sh = atof(*++argv);
            if (sl < 0) sl = 0;
            break;
          }
          goto usage;
        case 'l':
          if (--argc) {
            sl = atof(*++argv);
            if (sh < 0) sh = HUGE_VAL;
            break;
          }
          goto usage;
        default:
          goto usage;
        }
        break;
      case 'l':
        switch (*++*argv) {
        case 'h':
          if (--argc) {
            lh = atof(*++argv);
            if (ll < 0) ll = 0;
            break;
          }
          goto usage;
        case 'l':
          if (--argc) {
            ll = atof(*++argv);
            if (lh < 0) lh = HUGE_VAL;
            break;
          }
          goto usage;
        default:
          goto usage;
        }
        break;
      case 'x':
        switch (*++*argv) {
        case 'h':
          if (--argc) {
            xh = atoi(*++argv);
            if (xl < 0) xl = 0;
            break;
          }
          goto usage;
        case 'l':
          if (--argc) {
            xl = atoi(*++argv);
            if (xh < 0) xh = HUGE_VAL;
            break;
          }
          goto usage;
        default:
          goto usage;
        }
        break;
      case 'y':
        switch (*++*argv) {
        case 'h':
          if (--argc) {
            yh = atoi(*++argv);
            if (yl < 0) yl = 0;
            break;
          }
          goto usage;
        case 'l':
          if (--argc) {
            yl = atoi(*++argv);
            if (yh < 0) yh = HUGE_VAL;
            break;
          }
          goto usage;
        default:
          goto usage;
        }
        break;
      case 'r':
        switch (*++*argv) {
        case 'h':
          if (--argc) {
            rh = atof(*++argv);
            if (rl < 0.0) rl = 0.0;
            break;
          }
          goto usage;
        case 'l':
          if (--argc) {
            rl = atof(*++argv);
            if (rh < 0.0) rh = 1.0;
            break;
          }
          goto usage;
        default:
          goto usage;
        }
        break;
      default:
usage:
        fprintf(stderr, "Usage1: features [infile]\n\n");
        fprintf(stderr, "Usage2: features [options] [infile [outfile]]\n");
        fprintf(stderr, "  options\n");
        fprintf(stderr, "    [-sh n] [-sl n] [-lh n] [-ll n] [-rh n] [-rl n]\n");
        fprintf(stderr, "    [-xh n] [-xl n] [-yh n] [-yl n]\n");
        exit(-1);
      }
    } else {
      if (finame == NULL) {
        finame = *argv;
      } else if (foname == NULL && !(xh < 0 && yh < 0 && sh < 0.0 && rh < 0.0 && lh < 0.0)) {
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

  /* find max value, it shows number of objects */ 
  cnt = 0;
  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      if (cnt < pixel(image_in, j, i)) cnt = pixel(image_in, j, i);
    }
  }
  cnt -= (L_BASE - 1);

  /* get ready the features */
  if ((size = calloc(cnt, sizeof(double))) == NULL) {
    fprintf(stderr, "out of memory.\n");
    exit(-1);
  } 
  if ((ratio = calloc(cnt, sizeof(double))) == NULL) {
    fprintf(stderr, "out of memory.\n");
    exit(-1);
  } 
  if ((length = calloc(cnt, sizeof(double))) == NULL) {
    fprintf(stderr, "out of memory.\n");
    exit(-1);
  } 
  if ((gx = calloc(cnt, sizeof(int))) == NULL) {
    fprintf(stderr, "out of memory.\n");
    exit(-1);
  } 
  if ((gy = calloc(cnt, sizeof(int))) == NULL) {
    fprintf(stderr, "out of memory.\n");
    exit(-1);
  } 
  if ((lno = calloc(cnt, sizeof(int))) == NULL) {
    fprintf(stderr, "out of memory.\n");
    exit(-1);
  } 

  /* calculate the features */
  for (i = 0; i < cnt; i++) {
    size[i] = calc_size(i + L_BASE, &x, &y);
    gx[i] = x;
    gy[i] = y;
    length[i] = calc_length(i + L_BASE);
    ratio[i] = 4.0 * M_PI * size[i] / (length[i] * length[i]);
  }
    
  if (xh < 0 && yh < 0 && sh < 0.0 && rh < 0.0 && lh < 0.0) {
    printf("no.     S        L       R    G(x,y)\n");
    for (i = 0; i < cnt; i++) {
      printf("%3d %8.2f %8.2f %6.4f (%3d,%3d)\n",
              i, size[i], length[i], ratio[i], gx[i], gy[i]);
    }
    return (0);
  } else {
    for (i = 0, j = 0; i < cnt; i++) {
      if (xh >= 0 && (gx[i] > xh || gx[i] < xl)) {
        continue;
      }
      if (yh >= 0 && (gy[i] > yh || gy[i] < yl)) {
        continue;
      }
      if (sh >= 0 && (size[i] > sh || size[i] < sl)) {
        continue;
      }
      if (rh >= 0 && (ratio[i] > rh || ratio[i] < rl)) {
        continue;
      }
      if (lh >= 0 && (length[i] > lh || length[i] < ll)) {
        continue;
      }
      lno[j++] = L_BASE + i;
    }
    for (y = 0; y < image_in.y; y++) {
      for (x = 0; x < image_in.x; x++) {
        pixel(image_out, x, y) = 0;
        for (i = 0; i < j; i++) {
          if (pixel(image_in, x, y) == lno[i]) {
            pixel(image_out, x, y) = pixel(image_in, x, y);
          }
        }
      }
    }
    return (image_write(image_out, foname));
  }
}

double calc_size(int label, int *cx, int *cy)
{
  int i, j;
  double tx, ty, total;

  tx = ty = total = 0.0;
  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      if (pixel(image_in, j, i) == label) {
        tx += j;
        ty += i;
        total++;
      }
    }
  }
  if (total == 0.0) return(0.0);
  *cx = (int)(tx / total);
  *cy = (int)(ty / total);
  return(total);
}

double calc_length(int label)
{
  int i, j;
  double trace(int xs, int ys);

  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      if (pixel(image_in, j, i) == label) {
        return (trace(j-1, i));
      }
    }
  }
}

double trace(int xs, int ys)
{
  int x, y, no, vec;
  double len;

  len = 0;
  x = xs;
  y = ys;
  no = pixel(image_in, x+1, y);
  vec = 5;
  for (;;) {
    if (x == xs && y == ys && len != 0) return (len);
    switch (vec) {
    case 3:
      if (pixel(image_in, x+1, y) != no && pixel(image_in, x+1, y-1) == no) {
        x = x+1; y = y; len ++; vec = 0; continue;
      }
    case 4:
      if (pixel(image_in, x+1, y-1) != no && pixel(image_in, x, y-1) == no) {
        x = x+1; y = y-1; len += M_SQRT2; vec = 1; continue;
      }
    case 5:
      if (pixel(image_in, x, y-1) != no && pixel(image_in, x-1, y-1) == no) {
        x = x; y = y-1; len++; vec = 2; continue;
      }
    case 6:
      if (pixel(image_in, x-1, y-1) != no && pixel(image_in, x-1, y) == no) {
        x = x-1; y = y-1; len += M_SQRT2; vec = 3; continue;
      }
    case 7:
      if (pixel(image_in, x-1, y) != no && pixel(image_in, x-1, y+1) == no) {
        x = x-1; y = y; len++; vec = 4; continue;
      }
    case 0:
      if (pixel(image_in, x-1, y+1) != no && pixel(image_in, x, y+1) == no) {
        x = x-1; y = y+1; len += M_SQRT2; vec = 5; continue;
      }
    case 1:
      if (pixel(image_in, x, y+1) != no && pixel(image_in, x+1, y+1) == no) {
        x = x; y = y+1; len++; vec = 6; continue;
      }
    case 2:
      if (pixel(image_in, x+1, y+1) != no && pixel(image_in, x+1, y) == no) {
        x = x+1; y = y+1; len += M_SQRT2; vec = 7; continue;
      }
      vec = 3;
    }
  }
}

