#pragma once
#include "filtrado.h"

#define	BI_RGB	0
#define	BI_RLE8	1
#define	BI_RLE4	2
#define	BI_BITFIELDS	3
#define	BI_JPEG	4
#define	BI_PNG	5
#define	BI_ALPHABITFIELDS	6
#define	BI_CMYK	11
#define	BI_CMYKRLE8	12
#define	BI_CMYx1KRLE4	13

typedef struct __attribute__ ((packed)) bmpHeader_t{

    unsigned char fileId[2];
    unsigned int fileSize;
    unsigned short res1;
    unsigned short res2;
    unsigned int offsetStart;


}bmpHeader_t;

typedef struct __attribute__ ((packed)) dibHeader_t{

    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short numPlanes;
    unsigned short bpp;
    unsigned int compressMethod;
    unsigned int imageSize;
    unsigned int hRes;
    unsigned int vRes;
    unsigned int numColorsPalette;
    unsigned int numPrimaryColors;

}dibHeader_t;



typedef struct fileBMP_t{

    bmpHeader_t header;
    dibHeader_t attributes;
    rgbaByteColor_t *palette;
    int numChannels;

    unsigned char *data;

}fileBMP_t;

fileBMP_t* loadBMP(const char* fileName);
fileBMP_t* createBMP(int w, int h, int bpp);

void freeBMP(fileBMP_t* bmp);
int writeBMP(fileBMP_t* bmp, const char* fileName);