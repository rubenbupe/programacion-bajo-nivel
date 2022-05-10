#include "filtrado.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <cmath>

float convolucionSSE(float* data, float* kernel){

    float *vres=nullptr;
    float res=0.0f;
    vres = new float[3];

    asm volatile (
    "mov %[pv1], %%rax\n"
    "mov %[pv2], %%rbx\n"
    "mov %[vres], %%rdx\n"
    "mov $3, %%rcx\n"
    "loop:\n"
        "movaps (%%rax), %%xmm0\n"
        "movaps (%%rbx), %%xmm1\n"
        "mulps %%xmm0, %%xmm1\n"
        "haddps %%xmm1, %%xmm1\n"
        "haddps %%xmm1, %%xmm1\n"
        "movss %%xmm1,(%%rdx)\n"

        "add $16,%%rax\n"
        "add $16,%%rbx\n"
        "add $4,%%rdx\n"
        "dec %%rcx\n"
        "test %%rcx,%%rcx\n"
    "jnz loop\n"
        : [res] "=m" (res)
        : [pv1] "m" (data), [pv2] "m" (kernel), [vres] "m" (vres)

    );



    
    return vres[0] + vres[1] + vres[2];

}

void aplicaFiltroBilinearThreadSSE(float* data, int numChannels, int w, int h, int ini, int end, float* &dataOut){

    mmx_vector bilFilter = { .data{     1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f, 
                                        1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f, 
                                        1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f,
                                        0.0f     , 0.0f,      0.0f       }};
    
    float *kernel = new (std::align_val_t(16))float[12];
    float *r = new (std::align_val_t(16))float[12];
    float *g = new (std::align_val_t(16))float[12];
    float *b = new (std::align_val_t(16))float[12];
    float *a = new (std::align_val_t(16))float[12];
    memcpy(kernel, & bilFilter, sizeof(float) * 12);

    if (numChannels == 3){
            frgbColor_t *pixels = (frgbColor_t*)data;
            frgbColor_t *pixelsOut = new frgbColor_t[w * (end - ini)];

            for (int i = ini; i < end; i++){
            for (int j = 0; j < w; j++){
                frgbColor_t pixelAux;
                //pixelAux.r = pixelAux.g = pixelAux.b = 0;

                int cont = 0;

                for(int yf = -1; yf < 2; yf++){
                for(int xf = -1; xf < 2; xf++){
                    int y = (i + yf) % h;
                    int x = (j + xf) % w;

                    r[cont] = pixels[y * w + x].r;
                    g[cont] = pixels[y * w + x].g;
                    b[cont] = pixels[y * w + x].b;

                    cont ++;
                }
                }
                for(int x = 9; x < 12; x++){
                    r[cont] = 0;
                    g[cont] = 0;
                    b[cont] = 0;
                }


                pixelAux.r = convolucionSSE(r, kernel);
                pixelAux.g = convolucionSSE(g, kernel);
                pixelAux.b = convolucionSSE(b, kernel);

                pixelsOut[((i - ini) * w) + j] = pixelAux;
                
            }
            }

        memcpy(((frgbColor_t *)dataOut) + (ini * w), pixelsOut, sizeof(frgbColor_t) * (end - ini) * w);

    } else if (numChannels == 4){
            frgbaColor_t *pixels = (frgbaColor_t*)data;
            frgbaColor_t *pixelsOut = new frgbaColor_t[w * (end - ini)];

            for (int i = ini; i < end; i++){
            for (int j = 0; j < w; j++){
                frgbaColor_t pixelAux;
                int cont = 0;

                for(int yf = -1; yf < 2; yf++){
                for(int xf = -1; xf < 2; xf++){
                    int y = (i + yf) % h;
                    int x = (j + xf) % w;

                    r[cont] = pixels[y * w + x].r;
                    g[cont] = pixels[y * w + x].g;
                    b[cont] = pixels[y * w + x].b;
                    a[cont] = pixels[y * w + x].a;

                    cont ++;
                }
                }
                for(int x = 9; x < 12; x++){
                    r[cont] = 0;
                    g[cont] = 0;
                    b[cont] = 0;
                    a[cont] = 0;
                }


                pixelAux.r = convolucionSSE(r, kernel);
                pixelAux.g = convolucionSSE(g, kernel);
                pixelAux.b = convolucionSSE(b, kernel);
                pixelAux.a = convolucionSSE(a, kernel);


                pixelsOut[((i - ini) * w) + j] = pixelAux;
                
            }
            } 
        memcpy(((frgbaColor_t *)dataOut) + (ini * w), pixelsOut, sizeof(frgbaColor_t) * (end - ini) * w);
    }
} 

void aplicaFiltroBilinearThread(float* data, int numChannels, int w, int h, int ini, int end, float* &dataOut){

    float bilFilter[9] = {  1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f, 
                            1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f, 
                            1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f  };
    
    if (numChannels == 3){
            frgbColor_t *pixels = (frgbColor_t*)data;
            frgbColor_t *pixelsOut = new frgbColor_t[w * (end - ini)];

            for (int i = ini; i < end; i++){
            for (int j = 0; j < w; j++){
                frgbColor_t pixelAux;
                pixelAux.r = pixelAux.g = pixelAux.b = 0;

                for(int yf = -1; yf < 2; yf++){
                for(int xf = -1; xf < 2; xf++){
                    int y = (i + yf) % h;
                    int x = (j + xf) % w;

                    pixelAux.r += pixels[y * w + x].r * bilFilter[(yf + 1) * 3 + (xf + 1)];
                    pixelAux.g += pixels[y * w + x].g * bilFilter[(yf + 1) * 3 + (xf + 1)];
                    pixelAux.b += pixels[y * w + x].b * bilFilter[(yf + 1) * 3 + (xf + 1)];

                }
                }

                pixelsOut[((i - ini) * w) + j] = pixelAux;
                
            }
            }
        memcpy(((frgbColor_t *)dataOut) + (ini * w), pixelsOut, sizeof(frgbColor_t) * (end - ini) * w);
    } else if (numChannels == 4){
            frgbaColor_t *pixels = (frgbaColor_t*)data;
            frgbaColor_t *pixelsOut = new frgbaColor_t[w * (end - ini)];

            for (int i = ini; i < end; i++){
            for (int j = 0; j < w; j++){
                frgbaColor_t pixelAux;
                pixelAux.r = pixelAux.g = pixelAux.b = pixelAux.a = 0;

                for(int yf = -1; yf < 2; yf++){
                for(int xf = -1; xf < 2; xf++){
                    int y = (i + yf) % h;
                    int x = (j + xf) % w;

                    pixelAux.r += pixels[y * w + x].r * bilFilter[(yf+1)*4+(xf+1)];
                    pixelAux.g += pixels[y * w + x].g * bilFilter[(yf+1)*4+(xf+1)];
                    pixelAux.b += pixels[y * w + x].b * bilFilter[(yf+1)*4+(xf+1)];
                    pixelAux.a += pixels[y * w + x].a * bilFilter[(yf+1)*4+(xf+1)];
                }
                }

                pixelsOut[((i - ini) * w) + j] = pixelAux;
                
            }
            } 
        memcpy(((frgbaColor_t *)dataOut) + (ini * w), pixelsOut, sizeof(frgbaColor_t) * (end - ini) * w);
    }
}

void aplicaFiltroBilinear(float* data, int numChannels, int w, int h, float* &dataOut){
    int dataSize= h;
    int blockSize=dataSize/NUM_CPUS;
    std::vector<std::thread*> threads;

    float *dataOutTmp = new float[w * h * numChannels];
 

    for(int i = 0; i < NUM_CPUS; i++){
        //std::cout << (i * blockSize) + ((i != NUM_CPUS - 1) ? blockSize : (dataSize - (blockSize * (NUM_CPUS - 1)))) << "\n";
        threads.push_back(
            new std::thread(
                aplicaFiltroBilinearThread,
                std::ref(data),
                numChannels, 
                w, 
                h, 
                i * blockSize,
                (i * blockSize) + ((i != NUM_CPUS - 1) ? blockSize : (dataSize - (blockSize * (NUM_CPUS - 1)))),
                std::ref(dataOutTmp)
            )
        );
    }

    for(std::vector<std::thread*>::size_type i = 0; i != threads.size(); i++) {
        threads[i]->join();
        //memcpy(dataOut + (blockSize * i * w), tmpPixels[i], sizeof(float) * blockSize * w);
    }

    /* for(auto it=threads.begin(); 
            it!=threads.end();
            it++){
        (*it)->join();
    } */


    dataOut = (float*) dataOutTmp;

}


void charToFloat(unsigned char* data, rgbaByteColor_t* palette, int w, int h, int numChannels, int bpp , float* &dataOut){

    if(palette == nullptr){ // 24bpp, 32bpp
        dataOut = new float[w*h*numChannels];
        for(int i = 0; i < w*h*numChannels; i++){
            dataOut[i] = ((float) data[i])/256.0f;
        }
    } else { // 1bpp, 2bpp, 4bpp, 8bpp
        dataOut = new float[w*h*3];
        for(int i = 0; i < w*h; i++){
            int nSteps = (8 / bpp);
            int x = i / nSteps; // byte
            int y = bpp * (nSteps - (i % nSteps) - 1); // bit



            int idx = (data[x] >> y) % ((int) pow(2, bpp));



            dataOut[i*3  ] = ((float) palette[idx].r)/256.0f;
            dataOut[i*3+1] = ((float) palette[idx].g)/256.0f;
            dataOut[i*3+2] = ((float) palette[idx].b)/256.0f;
        }
    }
}


// 00001101  00001111    11111111
//   15         15            255

// 11010000  00001111    11011111
//   208        15            223

void floatToChar(float* data, rgbaByteColor_t* palette, int w, int h, int numChannels, int bpp, unsigned char* &dataOut){
    int dataOutCount = 0;


    if(palette != nullptr){

        dataOut = new unsigned char[w*h*3];
        //for(int i = 0; i < w*h*4; i+=2){ // Se transforma a 24bpp
        for(int i = 0; i < w*h*3; i++){
            //unsigned char p1 = (unsigned char)((int)((16.0f * data[i]))) << 4;
            //unsigned char p2 = (unsigned char)((int)((16.0f * data[i + 1])));
            //std::cout << i << ' ';
            dataOut[dataOutCount++] = (unsigned char)(256.0f * data[i]);
        }
    }else{
        dataOut = new unsigned char[w*h*numChannels];
        for(int i = 0; i < w*h*numChannels; i+=1){
            dataOut[dataOutCount++] = (unsigned char)(256.0f * data[i]);
        }
    }
}