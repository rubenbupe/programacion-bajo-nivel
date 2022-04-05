// catalan++17
#include "bmp.h"
#include "filtrado.h"
#include <iostream>
#include <time.h>

int main(int argc, char **argv){
    cudaSetDevice(0);
    fileBMP_t *bmp = loadBMP("./entrada/FLAG_B24.bmp");

    std::cout << "width: " << bmp->attributes.width << " height: " << bmp->attributes.height << " bpp: " << bmp->attributes.bpp << '\n';

    writeBMP(bmp, "./salida/salida.bmp");

    fileBMP_t* bmp2 = createBMP(bmp->attributes.width, bmp->attributes.height, bmp->attributes.bpp);
    bmp2->header = bmp->header;
    bmp2->attributes = bmp->attributes;

    float *data;
    float *dataOut;

    charToFloat(bmp->data, nullptr, bmp->attributes.width, bmp->attributes.height, data);

    clock_t begin = clock();
    aplicaFiltroBilinear(data, 3, bmp->attributes.width, bmp->attributes.height, dataOut);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    std::cout << "tiempo: " << time_spent << std::endl;

    floatToChar(dataOut, bmp2->attributes.width, bmp2->attributes.height, 3, bmp2->data);

    writeBMP(bmp2, "./salida/salida2.bmp");

    freeBMP(bmp);
    freeBMP(bmp2);
}