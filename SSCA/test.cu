/*
author: cltian
date: 2018/4/11
refs:
     http://answers.opencv.org/question/89050/passing-an-array-of-cvgpumat-to-a-cuda-kernel/
     http://answers.opencv.org/question/8466/how-access-gpumat-in-a-kernel/
     http://answers.opencv.org/question/26059/using-gpu-module-with-own-code/
     https://stackoverflow.com/questions/6965465/how-to-convert-gpumat-to-cvmat-in-opencv
     https://hk.saowen.com/a/43287d4469fb06d8e398c2b78572e8165133d687cac9082916ee12c69b44131d
*/

#include "test.h"
#include "helper_math.h"

using namespace cv;
using namespace cv::gpu;

__global__ void solveAllKernel(PtrStepSz<double>* pdSrc, double* sWgt, int cA1, int cA2, int cA3, int lvl) {
                
  int d = threadIdx.x + blockDim.x * blockIdx.x;
  int y = threadIdx.y + blockDim.y * blockIdx.y;
  int x = threadIdx.z + blockDim.z * blockIdx.z;

  if (d<cA1 && y<cA2 && x<cA3){
      int curY = y;
      int curX = x;
      int curD = d;
      double sum = 0;
      
      for( int s = 0; s < lvl; s ++ ) {
        //int idx = curY*imgW+curX;
        double curCost = pdSrc[s*cA1+curD].ptr(curY)[curX];
        //double curCost = smPyr[ s ]->costVol[ curD ].at<double>( curY, curX );
        sum += sWgt[ s ] * curCost;
        curY = curY / 2;
        curX = curX / 2;
        curD = ( curD + 1 ) / 2;
      }
      //int idxAss = assY*imgW+assX;
      pdSrc[m*cA1+d].ptr(y)[x] = sum;
      //newCosts[ m ][ assD ].at<double>( assY, assX ) = sum;
  }
}

