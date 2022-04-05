#include "filtrado.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>
#include <vector>
#include <mutex>



__global__ void aplicaFiltroBilinearGPU(float* data, float* bilFilter, int w, int h, int numChannels, float* dataOut){
    int thId = blockIdx.x * blockDim.x + threadIdx.x;
    if(thId >= w * h)
        return;

    int i = thId / w;
    int j = thId % w;
    

    for(int z = 0; z < numChannels; z++){
        dataOut[((i * w + j) * numChannels) + z] = 0.0f;
        for(int yf = -1; yf < 2; yf++){
            for(int xf = -1; xf < 2; xf++){
                int y = (i + yf) % h;
                int x = (j + xf) % w;
                dataOut[((i * w + j) * numChannels )+ z] += data[((y * w + x) * numChannels )+ z] * bilFilter[(yf + 1) * 3 + (xf + 1)];
            }
        }
    }
}

void aplicaFiltroBilinear(float* data, int numChannels, int w, int h, float* &dataOut){
    int dataSize = h * w;
    int colorSize;
    float *d_data;
    float *d_bilFilter;
    float *d_dataOut;

    float bilFilter[9] = {  1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f, 
                            1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f, 
                            1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f  };

    if(numChannels == 3){
        colorSize = sizeof(frgbColor_t);
    }else if(numChannels == 4){
        colorSize = sizeof(frgbaColor_t);
    }

    float *dataOutTmp = new float[w * h * colorSize / sizeof(float)];

    cudaMalloc(&d_data, dataSize * colorSize);
    cudaMalloc(&d_dataOut, dataSize * colorSize);
    cudaMalloc(&d_bilFilter, sizeof(float) * 9);
 
    cudaMemcpy(d_data, data, dataSize * colorSize, cudaMemcpyHostToDevice);
    // No lo hacemos en la GPU porque son solo 9 floats
    cudaMemcpy(d_bilFilter, bilFilter, sizeof(float) * 9, cudaMemcpyHostToDevice);


    int blockSize = 512;
	int numBlocks = dataSize/blockSize;
    numBlocks ++;

    printf("%d %d %d", dataSize, blockSize, numBlocks);

    aplicaFiltroBilinearGPU<<<numBlocks, blockSize >>>(d_data, d_bilFilter, w, h, numChannels, d_dataOut);
	cudaMemcpy(dataOutTmp, d_dataOut,  dataSize * colorSize , cudaMemcpyDeviceToHost);
	cudaDeviceSynchronize();

    dataOut = (float*) dataOutTmp;

}



void charToFloat(unsigned char* data, rgbaByteColor_t* palette, int w, int h, float* &dataOut){

    if(palette == nullptr){
        dataOut = new float[w*h*3];
        for(int i = 0; i < w*h*3; i++){
            dataOut[i] = ((float) data[i])/256.0f;
        }
    } else {
        dataOut = new float[w*h*4];
        for(int i = 0; i < w*h; i++){
            dataOut[i*4  ] = ((float) palette[data[i]].r)/256.0f;
            dataOut[i*4+1] = ((float) palette[data[i]].g)/256.0f;
            dataOut[i*4+2] = ((float) palette[data[i]].b)/256.0f;
            dataOut[i*4+3] = ((float) palette[data[i]].a)/256.0f;
        }
    }
}



void floatToChar(float* data, int w, int h, int numChannels, unsigned char* &dataOut){
    dataOut = new unsigned char[w*h*3];
    int dataOutCount = 0;
    for(int i = 0; i < w*h*numChannels; i+=numChannels){
        dataOut[dataOutCount++] = (unsigned char)(256.0f * data[i]);
        dataOut[dataOutCount++] = (unsigned char)(256.0f * data[i+1]);
        dataOut[dataOutCount++] = (unsigned char)(256.0f * data[i+2]);
    }
}