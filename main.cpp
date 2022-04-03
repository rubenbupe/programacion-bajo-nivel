// catalan++17
#include "bmp.h"
#include "filtrado.h"
#include <iostream>

int main(int argc, char **argv){
    fileBMP_t *bmp = loadBMP("./entrada/FLAG_B24.bmp");

    std::cout << "width: " << bmp->attributes.width << " height: " << bmp->attributes.height << " bpp: " << bmp->attributes.bpp << '\n';

    writeBMP(bmp, "./salida/salida.bmp");

    fileBMP_t* bmp2 = createBMP(bmp->attributes.width, bmp->attributes.height, bmp->attributes.bpp);
    bmp2->header = bmp->header;
    bmp2->attributes = bmp->attributes;

    float *data;
    float *dataOut;

    charToFloat(bmp->data, nullptr, bmp->attributes.width, bmp->attributes.height, data);
    aplicaFiltroBilinear(data, 3, bmp->attributes.width, bmp->attributes.height, dataOut);
    floatToChar(dataOut, bmp2->attributes.width, bmp2->attributes.height, 3, bmp2->data);

    writeBMP(bmp2, "./salida/salida2.bmp");

    freeBMP(bmp);
    freeBMP(bmp2);
}