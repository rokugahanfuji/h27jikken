#include <stdio.h>

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

void get_hsv(int r, int g, int b, int *h, int *s, int *v){
    int max = r > g ? (r > b ? r : b) : (g > b ? g : b);
    int min = r < g ? (r < b ? r : b) : (g < b ? g : b); 

    *h = get_h(r, g, b, max, min);
    *s = (max - min) * 255 / max;
    *v = max;
}

void main (){
    int r = 184;
    int g = 86;
    int b = 182;
    int h,s,v;
    get_hsv(r,g,b,&h,&s,&v);

    printf("%3d %3d %3d",h,s,v);
}