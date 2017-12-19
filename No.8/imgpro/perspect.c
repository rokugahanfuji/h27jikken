#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "params.h"

void param_pers(double k[], double a, double b, double x0, double y0, double z0, double z, double x, double y, double t, double s, int xs, int ys);

main(int argc, char **argv)
{
  char *finame = NULL, *foname = NULL;
  Image image_in, image_out;

  double zx = 1.0, zy = 1.0, px = 0.0, py = 0.0, pz = 0.0;
  double rx = 0.0, ry = 0.0, rz = 0.0, v = 10.0, s = 10.0;

  int i, j, m, n, d;
  int xs, ys;
  double x, y, w, p, q;
  double k[9];

  while (--argc) {
    if (**++argv == '-') {
      switch (*++*argv) {
      case 'p':
        switch (*++*argv) {
        case 'x':
          if (--argc) {
            px = atof(*++argv);
            break;
          }
	  goto usage;
        case 'y':
          if (--argc) {
            py = atof(*++argv);
            break;
          }
	  goto usage;
        case 'z':
          if (--argc) {
            pz = atof(*++argv);
            break;
          }
	  goto usage;
        default:
          goto usage;
        }
        break;
      case 'z':
        switch (*++*argv) {
        case 'x':
          if (--argc) {
            zx = atof(*++argv);
            break;
          }
	  goto usage;
        case 'y':
          if (--argc) {
            zy = atof(*++argv);
            break;
          }
	  goto usage;
        default:
	  goto usage;
        }
        break;
      case 'r':
        switch (*++*argv) {
        case 'x':
          if (--argc) {
            rx = atof(*++argv);
            break;
          }
	  goto usage;
        case 'y':
          if (--argc) {
            ry = atof(*++argv);
            break;
          }
	  goto usage;
        case 'z':
          if (--argc) {
            rz = atof(*++argv);
            break;
          }
	  goto usage;
        default:
	  goto usage;
        }
        break;
      case 'v':
        if (--argc) {
          v = atof(*++argv);
          break;
        }
	goto usage;
      case 's':
        if (--argc) {
          s = atof(*++argv);
          break;
        }
	goto usage;
      default:
usage:
        fprintf(stderr, "Usage: perspect [options] [-[infile [outfile]]\n");
        fprintf(stderr, "  options\n");
        fprintf(stderr, "    [-zx x_scale] [-zy y_scale]\n");
        fprintf(stderr, "    [-px x_move] [-py y_move] [-pz z_move]\n");
        fprintf(stderr, "    [-rz z_deg] [-rx x_deg] [-ry y_deg]\n");
        fprintf(stderr, "    [-v view] [-s screen]\n");
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
  xs = image_in.x / 2;
  ys = image_in.y / 2;
  image_out = mkimage(image_in.x, image_in.y);
  if (image_out.x == 0) return (-1);
  
  param_pers(k, zx, zy, px, py, pz, rz, rx, ry, v, s, xs, ys);
  for (i = -ys; i < ys; i++) {
    for (j = -xs; j < xs; j++) {
      w = k[0] * j + k[1] * i + k[2];
      x = k[3] * j + k[4] * i + k[5];
      y = k[6] * j + k[7] * i + k[8];
      x = x / w;
      y = y / w;
      if (y > 0) {
        m = y;
      } else {
        m = y - 1;
      }
      if (x > 0) {
        n = x;
      } else {
        n = x - 1;
      }
      q = y - m;
      p = x - n;
      if ((m >= -ys) && (m < ys) && (n >= -xs) && (n <xs)) {
        d = (1.0 - q)*((1.0 - p) * pixel(image_in, n+xs, m+ys)
                            + p  * pixel(image_in, n+1+xs, m+ys))
                 + q *((1.0 - p) * pixel(image_in, n+xs, m+1+ys)
                            + p  * pixel(image_in, n+1+xs, m+1+ys));
      } else {
        d = 0;
      }
      if (d < LOW) d = LOW;
      if (d > HIGH) d = HIGH;
      pixel(image_out, j+xs, i+ys) = d;
    }
  }
  return (image_write(image_out, foname));
}

void param_pers(double k[], double a, double b, double x0, double y0, double z0, double z, double x, double y, double t, double s, int xs, int ys)
{
  double l[4][4], m[4][4], n[4][4], k1, k2, k3, k4, k5, k6, k7, k8, k9;
  double u, v, w;
  int i;

  void matrix(double l[4][4], double m[4][4], double n[4][4]);

  u = x * M_PI / 180.0;
  v = y * M_PI / 180.0;
  w = z * M_PI / 180.0;
  l[0][0] = 1.0/xs;  l[0][1] = 0.0;     l[0][2] = 0.0;     l[0][3] = 0.0;
  l[1][0] = 0.0;     l[1][1] = -1.0/xs; l[1][2] = 0.0;     l[1][3] = 0.0;
  l[2][0] = 0.0;     l[2][1] = 0.0;     l[2][2] = 1.0;     l[2][3] = 0.0;
  l[3][0] = 0.0;     l[3][1] = 0.0;     l[3][2] = 0.0;     l[3][3] = 1.0;
  m[0][0] = a;       m[0][1] = 0.0;     m[0][2] = 0.0;     m[0][3] = 0.0;
  m[1][0] = 0.0;     m[1][1] = b;       m[1][2] = 0.0;     m[1][3] = 0.0;
  m[2][0] = 0.0;     m[2][1] = 0.0;     m[2][2] = 1.0;     m[2][3] = 0.0;
  m[3][0] = 0.0;     m[3][1] = 0.0;     m[3][2] = 0.0;     m[3][3] = 1.0;
  matrix(l, m, n);
  l[0][0] = 1.0;     l[0][1] = 0.0;     l[0][2] = 0.0;     l[0][3] = 0.0;
  l[1][0] = 0.0;     l[1][1] = 1.0;     l[1][2] = 0.0;     l[1][3] = 0.0;
  l[2][0] = 0.0;     l[2][1] = 0.0;     l[2][2] = 1.0;     l[2][3] = 0.0;
  l[3][0] = x0;      l[3][1] = y0;      l[3][2] = z0;      l[3][3] = 1.0;
  matrix(n, l, m);
  n[0][0] = cos(w);  n[0][1] = sin(w);  n[0][2] = 0.0;     n[0][3] = 0.0;
  n[1][0] = -sin(w); n[1][1] = cos(w);  n[1][2] = 0.0;     n[1][3] = 0.0;
  n[2][0] = 0.0;     n[2][1] = 0.0;     n[2][2] = 1.0;     n[2][3] = 0.0;
  n[3][0] = 0.0;     n[3][1] = 0.0;     n[3][2] = 0.0;     n[3][3] = 1.0;
  matrix(m, n, l);
  m[0][0] = 1.0;     m[0][1] = 0.0;     m[0][2] = 0.0;     m[0][3] = 0.0;
  m[1][0] = 0.0;     m[1][1] = cos(u);  m[1][2] = sin(u);  m[1][3] = 0.0;
  m[2][0] = 0.0;     m[2][1] = -sin(u); m[2][2] = cos(u);  m[2][3] = 0.0;
  m[3][0] = 0.0;     m[3][1] = 0.0;     m[3][2] = 0.0;     m[3][3] = 1.0;
  matrix(l, m, n);
  l[0][0] = cos(v);  l[0][1] = 0.0;     l[0][2] = sin(v);  l[0][3] = 0.0;
  l[1][0] = 0.0;     l[1][1] = 1.0;     l[1][2] = 0.0;     l[1][3] = 0.0;
  l[2][0] = -sin(v); l[2][1] = 0.0;     l[2][2] = cos(v);  l[2][3] = 0.0;
  l[3][0] = 0.0;     l[3][1] = 0.0;     l[3][2] = 0.0;     l[3][3] = 1.0;
  matrix(n, l, m);
  n[0][0] = 1.0;     n[0][1] = 0.0;     n[0][2] = 0.0;     n[0][3] = 0.0;
  n[1][0] = 0.0;     n[1][1] = 1.0;     n[1][2] = 0.0;     n[1][3] = 0.0;
  n[2][0] = 0.0;     n[2][1] = 0.0;     n[2][2] = -1.0;    n[2][3] = 0.0;
  n[3][0] = 0.0;     n[3][1] = 0.0;     n[3][2] = t;       n[3][3] = 1.0;
  matrix(m, n, l);
  m[0][0] = 1.0;     m[0][1] = 0.0;     m[0][2] = 0.0;     m[0][3] = 0.0;
  m[1][0] = 0.0;     m[1][1] = 1.0;     m[1][2] = 0.0;     m[1][3] = 0.0;
  m[2][0] = 0.0;     m[2][1] = 0.0;     m[2][2] = 1.0/s;   m[2][3] = 1.0/s;
  m[3][0] = 0.0;     m[3][1] = 0.0;     m[3][2] = -1.0;    m[3][3] = 0.0;
  matrix(l, m, n);
  l[0][0] = xs;      l[0][1] = 0.0;     l[0][2] = 0.0;     l[0][3] = 0.0;
  l[1][0] = 0.0;     l[1][1] = -xs;     l[1][2] = 0.0;     l[1][3] = 0.0;
  l[2][0] = 0.0;     l[2][1] = 0.0;     l[2][2] = 1.0;     l[2][3] = 0.0;
  l[3][0] = 0.0;     l[3][1] = 0.0;     l[3][2] = 0.0;     l[3][3] = 1.0;
  matrix(n, l, m);
  k1 = m[0][3];  k2 = m[1][3];  k3 = m[3][3];
  k4 = m[0][0];  k5 = m[1][0];  k6 = m[3][0];
  k7 = m[0][1];  k8 = m[1][1];  k9 = m[3][1];
  k[0] = k7 * k2 - k8 * k1;
  k[1] = k5 * k1 - k4 * k2;
  k[2] = k4 * k8 - k7 * k5;
  k[3] = k8 * k3 - k9 * k2;
  k[4] = k6 * k2 - k5 * k3;
  k[5] = k5 * k9 - k8 * k6;
  k[6] = k9 * k1 - k7 * k3;
  k[7] = k4 * k3 - k6 * k1;
  k[8] = k7 * k6 - k4 * k9;
}

void matrix(double l[4][4], double m[4][4], double n[4][4])
{
  int i, j, k;
  double p;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      p = 0;
      for (k = 0; k < 4; k++) {
        p += l[i][k] * m[k][j];
      }
      n[i][j] = p;
    }
  }
}
  
