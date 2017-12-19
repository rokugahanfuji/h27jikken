#include<stdio.h>
#include"ppmlib.h"
void		extract_regions(int ns, int nd);

int 
main(void)
{
	load_color_image(0, "");/* ファイル → 画像No.0 */
	extract_regions(0, 1);	/* No.0中の赤領域をNo.1へ */
	save_color_image(1, "");/* 画像No.1 → ファイル */
	return 0;
}

void 
extract_regions(int ns, int nd)
/* 画像No.ns中の領域を抽出して画像No.ndへ */
{
	int		i         , x, y;

	copy_color_image(ns, nd);
	for (y = 0; y < height[ns]; y++) {
		for (x = 0; x < width[ns]; x++) {
			for (i = 0; i < 3; i++) {
				image[nd][x][y][i] = image[ns][x][y][i];
			}
		}
	}
}
