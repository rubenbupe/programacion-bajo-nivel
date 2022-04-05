#pragma once

typedef struct frgbaColor_t{

    float r;
    float g;
    float b;
    float a;

}frgbaColor_t;

typedef struct frgbColor_t{

    float r;
    float g;
    float b;

}frgbColor_t;

typedef struct rgbaByteColor_t{

    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

}rgbaByteColor_t;

void aplicaFiltroBilinear(float* data, int numChannels, int w, int h, float* &dataOut);
void charToFloat(unsigned char* data, rgbaByteColor_t* palette, int w, int h, float* &dataOut);
void floatToChar(float* data, int w, int h, int numChannels, unsigned char* &dataOut);