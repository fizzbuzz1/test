#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>

using image_info = std::pair<byte*, size_t>;

class image{
private:
    HDC dcMem;
    HDC outDC;
    BITMAP bmp;
    HDC screenDC;
    std::vector<byte> buffer;
    std::vector<byte> output;

public:
    void savepng(const image_info bitmap);
    image_info screenshot();
};
