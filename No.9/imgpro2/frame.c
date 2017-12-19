#include <stdio.h>
#include <stdlib.h>
#include "params.h"

int
main(int argc, char **argv)
{
	char  *finame1 = NULL, *finame2 = NULL, *foname = NULL;
	Image	image_in1, image_in2, image_out;

	int	i, j, amp;

	while (--argc) {
		if (**++argv == '-') {
			switch (*++*argv) {
			case 'a':
				if (--argc) {
					amp = atof(*++argv);
					break;
				}
				goto usage;
			default:
		usage:
				fprintf(stderr, "Usage: motion [-a amp] [infile1 infile2 [outfile]]\n");
				exit(-1);
			}
		} else {
			if (finame1 == NULL) {
				finame1 = *argv;
			} else if (finame2 == NULL) {
				finame2 = *argv;
			} else if (foname == NULL) {
				foname = *argv;
			} else {
				goto usage;
			}
		}
	}

	image_in1 = image_read(finame1);
	if (image_in1.x == 0)
		return (-1);
	image_in2 = image_read(finame2);
	if (image_in2.x == 0)
		return (-1);
	image_out = mkimage(image_in1.x, image_in1.y);
	if (image_out.x == 0)
		return (-1);

	for (i = 1; i < image_in1.y - 1; i++) {
		for (j = 1; j < image_in1.x - 1; j++) {
			pixel(image_out, j, i) = 0;
			if (pixel(image_in1, j, i) == 255)
				if (pixel(image_in2, j, i) == 255)
					pixel(image_out, j, i) = 255;
		}
	}
	return (image_write(image_out, foname));
}
