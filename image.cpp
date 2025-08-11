#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


using std::vector;
using std::fstream;


void image::savepng(const image_info bitmap){
    int screenX = 1920;
    int screenY = 1080;
    int channels = 1;
    
    byte* loaded = stbi_load_from_memory(bitmap.first, bitmap.second, &screenX, &screenY, &channels, 0);
    stbi_write_png("out.png", screenX, screenY, channels, loaded, screenX * channels);
    stbi_image_free(loaded);
    MessageBoxA(NULL, "saved image", "info", MB_ICONINFORMATION | MB_OK);
}

image_info image::screenshot() {
    screenDC = GetDC(NULL);
    outDC = CreateCompatibleDC(outDC);
    dcMem = CreateCompatibleDC(screenDC);

    int screenX = GetDeviceCaps(screenDC, HORZRES) * 1.5;
    int screenY = GetDeviceCaps(screenDC, VERTRES) * 1.5;
            
    HBITMAP targetBMP = CreateCompatibleBitmap(screenDC, screenX, screenY);
    HBITMAP outputBMP = CreateCompatibleBitmap(screenDC, 1920, 1080);
    SelectObject(dcMem, targetBMP);
    SelectObject(outDC, outputBMP);

    if(!BitBlt(dcMem, 0,0, screenX, screenY, screenDC, 0,0, SRCCOPY)){
        MessageBoxA(NULL, "Failed", "info", MB_ICONERROR | MB_OK);
    }
            
    SetStretchBltMode(outDC, HALFTONE);
    StretchBlt(outDC, 0, 0, 1920, 1080, dcMem, 0, 0, screenX, screenY, SRCCOPY);

    GetObject(outputBMP, sizeof(BITMAP), &bmp);
        
    BITMAPFILEHEADER   bmfHeader;
    BITMAPINFOHEADER   bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmp.bmWidth;
    bi.biHeight = bmp.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((bmp.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmp.bmHeight;
    if(!buffer.size()){
        buffer.resize(dwBmpSize);
    }
    GetDIBits(outDC, outputBMP, 0, (UINT)bmp.bmHeight, (LPVOID)buffer.data(), (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfSize = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfType = 0x4D42;

    if(!output.size()){
        output.resize(bmfHeader.bfSize);
    }

    memcpy_s( (void*)output.data(), sizeof(BITMAPFILEHEADER), (void*)&bmfHeader, sizeof(BITMAPFILEHEADER) );
    memcpy_s( (void*)&output[sizeof(BITMAPFILEHEADER)], sizeof(BITMAPINFOHEADER), (void*)&bi, sizeof(BITMAPINFOHEADER) );
    memcpy_s( (void*)&output[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)], buffer.capacity(), (void*)buffer.data(), buffer.capacity() );
    return image_info(output.data(), output.capacity());
}