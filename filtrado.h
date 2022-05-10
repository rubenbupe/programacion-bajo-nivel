#pragma once

#define NUM_CPUS 8

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

typedef struct  __attribute__ ((packed)) rgbaByteColor_t{

    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

}rgbaByteColor_t;

typedef struct __attribute__((aligned(16))) mmx_vector{
    float data[12];
}mmx_vector;

float convolucionSSE(float* data, float* kernel);
void aplicaFiltroBilinearThread(float* data, int numChannels, int w, int h, int ini, int end, float* &dataOut);
void aplicaFiltroBilinear(float* data, int numChannels, int w, int h, float* &dataOut);
void charToFloat(unsigned char* data, rgbaByteColor_t* palette, int w, int h, int numChannels, int bpp, float* &dataOut);
void floatToChar(float* data, rgbaByteColor_t* palette, int w, int h, int numChannels, int bpp, unsigned char* &dataOut);