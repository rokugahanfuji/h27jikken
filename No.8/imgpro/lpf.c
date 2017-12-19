#include <math.h>
#include "params.h"
#include "lpf.h"

void lp_filter(Image image_in, Image image_out, double fx, double fy)
{
  Image image_buf;
  int i, j, ii, jj;
  int m, n;
  int i1, i2, j1, j2;
  double sinc, hanning;
  double coefh[MAX_TAP], coefv[MAX_TAP];
  double aa, bb, cc;
  double x, y;

  image_buf = mkimage(image_in.x, image_in.y);

  m = WINDOW / fx;
  if (m > MAX_TAP - 1) m = MAX_TAP - 1;
  coefh[0] = 1.0;
  for (i = 1; i <= m; i++) {
    x = (double)i * M_PI * fx;
    sinc = sin(x) / x;
    x = x / WINDOW;
    hanning = 0.5 + 0.5 * cos(x);
    coefh[i] = sinc * hanning;
  }
  n = WINDOW / fy;
  if (n > MAX_TAP - 1) n = MAX_TAP - 1;
  coefv[0] = 1.0;
  for (i = 1; i <= n; i++) {
    y = (double)i * M_PI * fy;
    sinc = sin(y) / y;
    y = y / WINDOW;
    hanning = 0.5 + 0.5 * cos(y);
    coefv[i] = sinc * hanning;
  }
  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      aa = 1;
      bb = pixel(image_in,j, i);
      for (jj = 1; jj <= m; jj++) {
        j1 = j - jj;
        j2 = j + jj;
        if (j1 < 0) j1 = 0;
        if (j1 > image_in.x - 1) j1 = image_in.x - 1;
        if (j2 < 0) j2 = 0;
        if (j2 > image_in.x - 1) j2 = image_in.x - 1;
        cc = pixel(image_in, j1, i) + pixel(image_in, j2, i);
        bb = bb + cc * coefh[jj];
        aa = aa + coefh[jj] * 2;
      }
      bb = bb / aa;
      if (bb < LOW) bb = LOW;
      if (bb > HIGH) bb =HIGH;
      pixel(image_out, j, i) = bb;
    }
  }
  for (i = 0; i < image_out.y; i++) {
    for (j = 0; j < image_out.x; j++) {
      pixel(image_in, i, j) = pixel(image_out, i, j);
    }
  }
  for (i = 0; i < image_in.y; i++) {
    for (j = 0; j < image_in.x; j++) {
      aa = 1;
      bb = pixel(image_in, j, i);
      for (ii = 1; ii <= n; ii++) {
        i1 = i - ii;
        i2 = i + ii;
        if (i1 < 0) i1 = 0;
        if (i1 > image_in.y - 1) i1 = image_in.y - 1;
        if (i2 < 0) i2 = 0;
        if (i2 > image_in.y - 1) i2 = image_in.y - 1;
        cc = pixel(image_in, j, i1) + pixel(image_in, j, i2);
        bb = bb + cc * coefv[ii];
        aa = aa + coefv[ii] * 2;
      }
      bb = bb / aa;
      if (bb < LOW) bb = LOW;
      if (bb > HIGH) bb = HIGH;
      pixel(image_out, j, i) = bb;
    }
  }
}
