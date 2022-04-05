#include "bmp.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

fileBMP_t* createBMP(int w, int h, int bpp){
    fileBMP_t* bmp = new fileBMP_t;
    memset(bmp, 0, sizeof(fileBMP_t));

    bmp->attributes.width = w;
    bmp->attributes.height = h;
    bmp->attributes.bpp = bpp;

    return bmp;

}


fileBMP_t* loadBMP(const char* fileName){
    fileBMP_t *bmp = new fileBMP_t;
    memset(bmp, 0, sizeof(fileBMP_t));

    //read cabecera 1    
    FILE *f = fopen(fileName, "rb");
    if(f == nullptr){
        //error
        free(bmp);
        return nullptr;
    }

    fread(&bmp->header, sizeof(bmpHeader_t), 1, f);

    //read tipo fichero
    int headerSize = 0;
    fread(&headerSize, sizeof(int), 1, f);
    if(headerSize != 40){
        //si no es 40 B, error y returno null
        //error
        free(bmp);
        fclose(f);
        return nullptr;
    }
    fseek(f, -4, SEEK_CUR);

    //read cabecera 2
    fread(&bmp->attributes, sizeof(dibHeader_t), 1, f);

    //si paleta
    if(bmp->attributes.bpp <= 8){
        bmp->palette = new rgbaColor_t[bmp->attributes.numColorsPalette];
        fread(bmp->palette, sizeof(rgbaColor_t), bmp->attributes.numColorsPalette, f);
    }
    std::cout << bmp->attributes.numColorsPalette << std::endl;

        //leer paleta
    //leer datos   
    int dataSize = (bmp->attributes.width * bmp->attributes.height * bmp->attributes.bpp) / 8;
    bmp->data = new unsigned char[dataSize];
    fread(bmp->data, 1, dataSize, f);
    fclose(f);

    return bmp;
}

void freeBMP(fileBMP_t* bmp){

    if(bmp->palette){
        delete[] bmp->palette;
    }
    if(bmp->data){
        delete[] bmp->data;
    }
    delete bmp;

}

int writeBMP(fileBMP_t* bmp, const char* fileName){
    FILE *f = fopen(fileName, "wb");
    if(f == nullptr){
        //error
        return -1;
    }

    fwrite(&bmp->header, sizeof(bmpHeader_t), 1, f);
    fwrite(&bmp->attributes, sizeof(dibHeader_t), 1, f);

    //si paleta
    if(bmp->attributes.bpp <= 8){
        //write paleta
        fwrite(bmp->palette, sizeof(rgbaColor_t), bmp->attributes.numColorsPalette, f);
    }
    //write datos   
    int dataSize = (bmp->attributes.width * bmp->attributes.height * bmp->attributes.bpp) / 8;
    fwrite(bmp->data, 1, dataSize, f);
    fclose(f);

    return 1;

}