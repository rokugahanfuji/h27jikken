#include<stdio.h>
#include"ppmlib.h"
void extract_regions(int ns, int nd );

int main(void)
{
    load_color_image( 0, "puzzles.ppm" ); /* ファイル → 画像No.0 */
    extract_regions( 0, 1 );   /* No.0中の赤領域をNo.1へ */
    save_color_image( 1, "result.ppm" ); /* 画像No.1 → ファイル */
    return 0;
}

void extract_regions(int ns, int nd )
/* 画像No.ns中の領域を抽出して画像No.ndへ */
{
    int i,x,y;

    copy_color_image(ns, nd);
    for(y = 0;y < height[ns];y++){
        for(x = 0;x < width[ns];x++){
            int r = image[ns][x][y][0];
            int g = image[ns][x][y][1];
            int b = image[ns][x][y][2];
            
            if (180 < r && r < 220 && 120 < g && g < 160 && 90 < b && b < 130)
                for (i=0;i<3;i++)
                    image[nd][x][y][i] = image[ns][x][y][i];
            else 
                for (i=0;i<3;i++)
                    image[nd][x][y][i] = 0;
        }
    }
}