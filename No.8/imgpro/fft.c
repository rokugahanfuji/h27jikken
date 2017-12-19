#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "params.h"

#define BUFF_MAX	128
#define OPT		1

void rvmtx(double a[], double b[], int xsize, int ysize);
void fft1(double *a_rl, double *a_im, int ex, int inv, double *sin_tbl, double *cos_tbl, double *wk);
void cstb(int length, int inv, double *sin_tbl, double *cos_tbl);
void birv(double *a, int length, int ex, double *b);

void fft2(double a_rl[], double a_im[], double b_rl[], double b_im[], int exh, int exv, int inv, double *sin_tbl, double *cos_tbl)
{
  int i, j, k, w, j1, j2, numb, lenb, timb, lenh = 1, lenv = 1;
  double xr, xi, yr, yi, nrml;

  for (i = 0; i < exh; i++) {
    lenh *= 2;
  }
  for (i = 0; i < exv; i++) {
    lenv *= 2;
  }
  if (b_rl == NULL || b_im == NULL) {
    fprintf(stderr, "memory overflow!\n");
    return;
  }
  for (i = 0; i < lenv; i++) {
    fft1(&(a_rl[i * lenh]), &(a_im[i * lenh]), exh, inv, sin_tbl, cos_tbl, b_rl);
  }
  rvmtx(a_rl, b_rl, lenh, lenv);
  rvmtx(a_im, b_im, lenh, lenv);
  for (i = 0; i < lenh; i++) {
    fft1(&(b_rl[i * lenv]), &(b_im[i * lenv]), exv, inv, sin_tbl, cos_tbl, a_rl);
  }
  rvmtx(b_rl, a_rl, lenv, lenh);
  rvmtx(b_im, a_im, lenv, lenh);
}

void rvmtx(double a[], double b[], int xsize, int ysize)
{
  int i, j;

  for (i = 0; i < ysize; i++) {
    for (j = 0; j < xsize; j++) {
      b[j * ysize + i] = a[i * xsize + j];
    }
  }
}

void fft1(double *a_rl, double *a_im, int ex, int inv, double *sin_tbl, double *cos_tbl, double *wk)
{
  int i, j, k, w, j1, j2;
  int numb, lenb, timb, length=1;
  double xr, xi, yr, yi, nrml;

  for(i = 0; i < ex; i++) {
    length *= 2;
  }
  cstb(length, inv, sin_tbl, cos_tbl);
  if (OPT == 1) {
    for (i = 1; i < length; i += 2) {
      a_rl[i] = -a_rl[i];
      a_im[i] = -a_im[i];
    }
  }
  numb = 1;
  lenb = length;
  for (i = 0; i < ex; i++) {
    lenb /= 2;
    timb = 0;
    for (j = 0; j < numb; j++) {
      w = 0;
      for (k = 0; k < lenb; k++) {
        j1 = timb + k;
        j2 = j1 + lenb;
        xr = a_rl[j1];
        xi = a_im[j1];
        yr = a_rl[j2];
        yi = a_im[j2];
        a_rl[j1] = xr + yr;
        a_im[j1] = xi + yi;
        xr = xr - yr;
        xi = xi - yi;
        a_rl[j2] = xr * cos_tbl[w] - xi * sin_tbl[w];
        a_im[j2] = xr * sin_tbl[w] + xi * cos_tbl[w];
        w += numb;
      }
      timb += (2 * lenb);
    }
    numb *= 2;
  }
  birv(a_rl, length, ex, wk);
  birv(a_im, length, ex, wk);
  if (OPT == 1) {
    for (i = 1; i < length; i += 2) {
      a_rl[i] = -a_rl[i];
      a_im[i] = -a_im[i];
    }
  }
  nrml = 1.0 / sqrt((double)length);
  for (i = 0; i < length; i++) {
    a_rl[i] *= nrml;
    a_im[i] *= nrml;
  }
}

void cstb(int length, int inv, double *sin_tbl, double *cos_tbl)
{
  int i;
  double xx, arg;

  xx = ((-M_PI) * 2.0) / (double)length;
  if (inv < 0) xx = -xx;
  for (i = 0; i < length; i++) {
    arg = (double)i * xx;
    sin_tbl[i] = sin(arg);
    cos_tbl[i] = cos(arg);
  }
}

void birv(double *a, int length, int ex, double *b)
{
  int i, ii, k, bit;

  for (i = 0; i < length; i++) {
    for (k = 0, ii = i, bit = 0; ; bit <<= 1, ii >>= 1) {
      bit = (ii & 1) | bit;
      if (++k == ex) break;
    }
    b[i] = a[bit];
  }
  for (i = 0; i < length; i++) {
    a[i] = b[i];
  }
}

/* To change the size to 2^x */
void adjust(Image image_in, Image image_out)
{
  int i, j, m, n, d;
  int xs, ys, xs_in, ys_in;
  double zx, zy;
  double x, y, p, q;

  xs_in = image_in.x / 2;
  ys_in = image_in.y / 2;
  xs = image_out.x / 2;
  ys = image_out.y / 2;
  zx = (double)image_out.x / (double)image_in.x;
  zy = (double)image_out.y / (double)image_in.y;

  for (i = -ys; i < ys; i++) {
    for (j = -xs; j < xs; j++) {
      y = i / zy;
      x = j / zx;
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
      if ((m >= -ys_in) && (m < ys_in) && (n >= -xs_in) && (n <xs_in)) {
	d = (1.0 - q)*((1.0 - p) * pixel(image_in, n+xs_in, m+ys_in)
                            + p  * pixel(image_in, n+1+xs_in, m+ys_in))
                 + q *((1.0 - p) * pixel(image_in, n+xs_in, m+1+ys_in)
                            + p  * pixel(image_in, n+1+xs_in, m+1+ys_in));
      } else {
	d = 0;
      }
      if (d < LOW) d = LOW;
      if (d > HIGH) d = HIGH;
      pixel(image_out, j+xs, i+ys) = d;
    }
  }
}
