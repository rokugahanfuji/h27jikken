#define HIGH		255
#define LOW		0
#define LEVEL		256
#define HIST_X_SIZE	(LEVEL/2)
#define HIST_Y_SIZE	(LEVEL/2)

typedef struct IMG_CELL {
  int x;
  int y;
  unsigned char *img;
} Image;

#define pixel(i, a, b)	(((i).img)[(b)*((i).x)+(a)])

Image mkimage(int xsiz, int ysiz);
Image image_read(char *fname);
int image_write(Image i, char *fname);
