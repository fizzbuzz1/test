#include "image.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace tesseract;

int main(){
    TessBaseAPI* api = new TessBaseAPI();
    if (api->Init(NULL, "eng")) {
        std::cout << "Failed\n";
        exit(1);
    }

    image img; 
    img.savepng(img.screenshot());
    std::fstream output_file;
    output_file.open("ocr_result.txt", std::ios_base::out);

    Pix* target = pixRead("out.png");
    api->SetImage(target);
    const char* ocr_output = api->GetUTF8Text();
    output_file << ocr_output;

    delete(target);
    delete(api);
    return 0;
}