#include<stdio.h>
#include"ppmlib.h"
void extract_regions(int ns, int nd);
//void extract_regions(int ns, int nd , int r1, int r2, int g1, int g2, int b1, int b2);

int main(void)
{
    load_color_image( 0, "faces.ppm" ); /* ファイル → 画像No.0 */
    char str[20] = {0};
    int r,g,b;
    int offset = 30;
    //190,140,120
    /*
    for (r=160;r<200;r=r+10){
        for (g=120;g<160;g=g+10){
            for (b=100;b<140;b=b+10){
                sprintf(str,"%03d%03d%03d.ppm",r,g,b);
                extract_regions( 0, 1 ,r,r+offset,g,g+offset,b,b+offset);   
                save_color_image( 1, str ); 
            }          
        }   
    }
    */
    extract_regions( 0, 1);
    save_color_image( 1, "result.ppm"); 
    return 0;
}

//void extract_regions(int ns, int nd , int r1, int r2, int g1, int g2, int b1, int b2)
/* 画像No.ns中の領域を抽出して画像No.ndへ */
void extract_regions(int ns, int nd)
{
    int i,x,y;

    copy_color_image(ns, nd);
    for(y = 0;y < height[ns];y++){
        for(x = 0;x < width[ns];x++){
            int r = image[ns][x][y][0];
            int g = image[ns][x][y][1];
            int b = image[ns][x][y][2];
            
            //190,220,140,170,120,150
            if (190 < r && r < 255 && 0 < g && g < 170 && 115 < b && b < 250)
                for (i=0;i<3;i++)
                    image[nd][x][y][i] = image[ns][x][y][i];
            else 
                for (i=0;i<3;i++)
                    image[nd][x][y][i] = 0;
        }
    }
}
