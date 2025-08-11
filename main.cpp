#include "image.h"

int main(){
    image img;
    img.savepng(img.screenshot());
    return 0;
}