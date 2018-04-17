#ifndef PROJECT_GUIDEDFILTER_H_H
#define PROJECT_GUIDEDFILTER_H_H

#include "headers.h"

//#define PY_LVL 5
__global__ void solveAllKernel(PtrStepSz<double>* pdSrc, double* sWgt, int cA1, int cA2, int cA3, int lvl);
#endif
