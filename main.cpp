// catalan++17
#include "bmp.h"
#include "filtrado.h"
#include <iostream>
#include <time.h>

int main(int argc, char **argv){
    fileBMP_t *bmp = loadBMP("./entrada/FLAG_B24.bmp");

    std::cout << "width: " << bmp->attributes.width << " height: " << bmp->attributes.height << " bpp: " << bmp->attributes.bpp << '\n';

    writeBMP(bmp, "./salida/salida.bmp");

    fileBMP_t* bmp2 = createBMP(bmp->attributes.width, bmp->attributes.height, bmp->attributes.bpp);
    bmp2->header = bmp->header;
    bmp2->attributes = bmp->attributes;
    bmp2->numChannels = bmp->numChannels;

    float *data;
    float *dataOut;


    charToFloat(bmp->data, bmp->palette, bmp->attributes.width, bmp->attributes.height, bmp->numChannels, bmp->attributes.bpp, data);

    aplicaFiltroBilinear(data, bmp->numChannels, bmp->attributes.width, bmp->attributes.height, dataOut);

    floatToChar(dataOut, nullptr, bmp2->attributes.width, bmp2->attributes.height, bmp2->numChannels, bmp->attributes.bpp, bmp2->data);
    if(bmp->palette != nullptr){
        bmp2->palette = nullptr;
        bmp2->attributes.numColorsPalette = 0;
        bmp2->attributes.bpp = 24;
        bmp2->numChannels = 3;
    }

    writeBMP(bmp2, "./salida/salida2.bmp");

    freeBMP(bmp);
    freeBMP(bmp2);
}