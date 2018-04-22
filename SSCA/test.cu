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


void caller_test(SSCA**& smPyr, double* invWgt, int lvl){
    	GpuMat* mats;
	const int constVar1 = PY_LVL*smPyr[ 0 ]->maxDis;
	const int constVar2 = smPyr[ 0 ]->maxDis;
	PtrStepSz<double>* phSrc = new PtrStepSz<double>[constVar1];
	PtrStepSz<double>* phDst = new PtrStepSz<double>[constVar1];
	PtrStepSz<double>* pdSrc;
	//PtrStepSz<double>* pdDst;
	double *dinvWgt  = new double[ PY_LVL * PY_LVL];
	int i = 0;
	for(int s=0; s<PY_LVL; s++)
		for(int d=0; d<constVar2; d++)
			mats[i++].upload(smPyr[s]->rcostVol[d]);
	for (int i=0; i<constVar1; i++)
		phSrc[i] = mats[i];
    cudaCheckError(cudaMalloc(&dinvWgt, sizeof(double)*PY_LVL*PY_LVL));
	cudaCheckError(cudaMalloc(&pdSrc, constVar1*sizeof(PtrStepSz<double>)));
	//cudaCheckError(cudaMalloc(&pdDst, constVar1*sizeof(PtrStepSz<double>)));
    cudaCheckError(cudaMemcpy(dinvWgt, invWgt, sizeof(double)*PY_LVL*PY_LVL, cudaMemcpyHostToDevice));
	cudaCheckError(cudaMemcpy(pdSrc, phSrc, constVar1*sizeof(PtrStepSz<double>), 
		                      cudaMemcpyHostToDevice));

    dim3 block(8, 8, 8);
    dim3 grid( (constVar2+block.x-1)/block.x, (hei+block.y-1)/block.y, (wid+block.z-1)/block.z);

	solveAllKernel<<<grid, block>>>(pdSrc, dinvWgt, constVar2, hei, wid, PY_LVL);

	cudaCheckError(cudaMemcpy(phDst, pdSrc, constVar1*sizeof(PtrStepSz<double>),
		                      cudaMemcpyDeviceToHost));
	for (int i=0; i<constVar1; i++)
		mats[i] = phDst[i];
	for( int s = 0; s < PY_LVL; s ++ ) {
		for( int d = 0; d < smPyr[ s ]->maxDis; d ++ ) {
			//smPyr[ s ]->costVol[ d ] = pDistData[ s*PY_LVL+d ]
			Mat tempMatrix;
		        mats[s*PY_LVL+d].download(tempMatrix);
		    smPyr[ s ]->costVol[ d ] = tempMatrix.clone();
		}
	}

	cudaCheckError(cudaFree(pdSrc));
	cudaCheckError(cudaFree(dinvWgt));
}


void caller_test(SSCA**& smPyr, double* invWgt, int lvl){
    	GpuMat* mats;
	const int constVar1 = PY_LVL*smPyr[ 0 ]->maxDis;
	const int constVar2 = smPyr[ 0 ]->maxDis;
	PtrStepSz<double>* phSrc = new PtrStepSz<double>[constVar1];
	PtrStepSz<double>* phDst = new PtrStepSz<double>[constVar1];
	PtrStepSz<double>* pdSrc;
	//PtrStepSz<double>* pdDst;
	double *dinvWgt  = new double[ PY_LVL * PY_LVL];
	int i = 0;
	for(int s=0; s<PY_LVL; s++)
		for(int d=0; d<constVar2; d++)
			mats[i++].upload(smPyr[s]->costVol[d]);
	for (int i=0; i<constVar1; i++)
		phSrc[i] = mats[i];
    cudaCheckError(cudaMalloc(&dinvWgt, sizeof(double)*PY_LVL*PY_LVL));
	cudaCheckError(cudaMalloc(&pdSrc, constVar1*sizeof(PtrStepSz<double>)));
	//cudaCheckError(cudaMalloc(&pdDst, constVar1*sizeof(PtrStepSz<double>)));
    cudaCheckError(cudaMemcpy(dinvWgt, invWgt, sizeof(double)*PY_LVL*PY_LVL, cudaMemcpyHostToDevice));
	cudaCheckError(cudaMemcpy(pdSrc, phSrc, constVar1*sizeof(PtrStepSz<double>), 
		                      cudaMemcpyHostToDevice));

    dim3 block(8, 8, 8);
    dim3 grid( (constVar2+block.x-1)/block.x, (hei+block.y-1)/block.y, (wid+block.z-1)/block.z);

	solveAllKernel<<<grid, block>>>(pdSrc, dinvWgt, constVar2, hei, wid, PY_LVL);

	cudaCheckError(cudaMemcpy(phDst, pdSrc, constVar1*sizeof(PtrStepSz<double>),
		                      cudaMemcpyDeviceToHost));
	for (int i=0; i<constVar1; i++)
		mats[i] = phDst[i];
	for( int s = 0; s < PY_LVL; s ++ ) {
		for( int d = 0; d < smPyr[ s ]->maxDis; d ++ ) {
			//smPyr[ s ]->costVol[ d ] = pDistData[ s*PY_LVL+d ]
			Mat tempMatrix;
		        mats[s*PY_LVL+d].download(tempMatrix);
		    smPyr[ s ]->costVol[ d ] = tempMatrix.clone();
		}
	}

	cudaCheckError(cudaFree(pdSrc));
	cudaCheckError(cudaFree(dinvWgt));
}

