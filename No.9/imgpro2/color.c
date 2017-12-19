#include<stdio.h>
#include"ppmlib.h"
void extract_regions(int ns, int nd);
void get_hsv(int r, int g, int b, int *h, int *s, int *v);
int get_h(int r, int g, int b, int max, int min);

int main(void)
{
    load_color_image( 0, "faces.ppm" ); /* ファイル → 画像No.0 */
    char str[20] = {0};
    int r,g,b;
    int offset = 30;
    extract_regions( 0, 1);
    save_color_image( 1, "result.ppm"); 
    int h,s,v;
    return 0;
}

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
            
            int h,s,v;
            get_hsv(r,g,b,&h,&s,&v);
            
            //190,220,140,170,120,150
            if (0 < h && h < 30 && 60 < s && s < 250 && 100 < v && v < 220)
                for (i=0;i<3;i++)
                    image[nd][x][y][i] = image[ns][x][y][i];
            else 
                for (i=0;i<3;i++)
                    image[nd][x][y][i] = 0;
        }
    }
}

void get_hsv(int r, int g, int b, int *h, int *s, int *v){
    int max = r > g ? (r > b ? r : b) : (g > b ? g : b);
    int min = r < g ? (r < b ? r : b) : (g < b ? g : b); 

    *h = get_h(r, g, b, max, min);
    *s = (max - min) * 255 / max;
    *v = max;
}

int get_h(int r, int g, int b, int max, int min){
    int h;

    if (r == g && g == b){
        h = 0;
    } else if (r >= g && r >= b){
        h = 60 * (g-b) / (max-min);
    } else if (g >= r && g >= b){
        h = 60 * (b-r) / (max-min) + 120;
    } else if (b >= r && b >= g){
        h = 60 * (r-g) / (max-min) + 240;
    }

    if (h < 0)
        h += 360;

    return h;
}