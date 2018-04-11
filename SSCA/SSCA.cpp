#include "CommFunc.h"
#include "SSCA.h"
using namespace cv;
using namespace cv::gpu;
//#include "test.h"
#include<stdio.h>
SSCA::SSCA(const Mat l, const Mat r, const int m, const int d)
	: lImg( l ), rImg( r ), maxDis( m ), disSc( d )
{
	// all input image must be in CV_64FC3 format
	CV_Assert( lImg.type() == CV_64FC3 && rImg.type() == CV_64FC3 );
	wid = lImg.cols;
	hei = lImg.rows;
	// init disparity map
	lDis = Mat::zeros( hei, wid, CV_8UC1 );
#ifdef COMPUTE_RIGHT
	rDis = Mat::zeros( hei, wid, CV_8UC1 );
	lSeg = Mat::zeros( hei, wid, CV_8UC3 );
	lChk = Mat::zeros( hei, wid, CV_8UC1 );
#endif
	// init cost volum data
	costVol = new Mat[ maxDis  ];
	for( int mIdx = 0; mIdx < maxDis; mIdx ++ ) {
		costVol[ mIdx ] = Mat::zeros( hei, wid, CV_64FC1 );
	}
#ifdef COMPUTE_RIGHT
	rCostVol = new Mat[ maxDis ];
	for( int mIdx = 0; mIdx < maxDis; mIdx ++ ) {
		rCostVol[ mIdx ] = Mat::zeros( hei, wid, CV_64FC1 );
	}
#endif
}



SSCA::~SSCA(void)
{
	delete [] costVol;
#ifdef COMPUTE_RIGHT
	delete [] rCostVol;
#endif
}
// get left disparity
Mat SSCA::getLDis()
{
	return lDis;
}
#ifdef COMPUTE_RIGHT
// get right disparity
Mat SSCA::getRDis()
{
	return rDis;
}
Mat SSCA::getLSeg()
{
	return lSeg;
}
Mat SSCA::getLChk()
{
	return lChk;
}
#endif
//
// Save Cost Volume
//
void SSCA::saveCostVol(const string fn)
{
	int myY = 53;
	int startX = 285;
	int endX   = 424;

	// save as matlab order
	FILE* fp = fopen( fn.c_str(), "w" );

	for( int d = 1; d < maxDis; d ++ ) {
		printf( "-s-v-" );
		for( int x = 0; x < wid; x ++ ) {
			for( int y = 0; y < hei; y ++ ) {
				if( y == myY && x >= startX && x <= endX ) {
					double* cost = costVol[ d ].ptr<double>( y );
					fprintf( fp, "%lf  ", cost[ x ] );
				}
			}
		}
		fprintf( fp, "\n" );
	}
	fclose( fp );
}

//
// Add previous cost volume from pyramid
//
void SSCA::AddPyrCostVol(SSCA *pre, const double COST_ALPHA )
{
	printf( "\n\tAdd Pyramid Cost: COST_ALPHA = %.2lf", COST_ALPHA );
	for( int d = 1; d < maxDis; d ++ ) {
		int pD = ( d + 1 ) / 2;
		printf( ".a.p." );
		for( int y = 0; y < hei; y ++ ) {
			int pY = y / 2;
			double* cost = costVol[ d ].ptr<double>( y );
			double* pCost = pre->costVol[ pD ].ptr<double>( pY );
			for( int x = 0; x < wid; x ++ ) {
				int pX = x / 2;
				cost[ x ] = COST_ALPHA * cost[ x ] +
					( 1 - COST_ALPHA ) * pCost[ pX ];

			}
		}
	}
#ifdef COMPUTE_RIGHT
	for( int d = 1; d < maxDis; d ++ ) {
		int pD = ( d + 1 ) / 2;
		printf( "r.a.p." );
		for( int y = 0; y < hei; y ++ ) {
			int pY = y / 2;
			double* cost = rCostVol[ d ].ptr<double>( y );
			double* pCost = pre->rCostVol[ pD ].ptr<double>( pY );
			for( int x = 0; x < wid; x ++ ) {
				int pX = x / 2;
				cost[ x ] = COST_ALPHA * cost[ x ] +
					( 1 - COST_ALPHA ) * pCost[ pX ];

			}
		}
	}
#endif
}
//
// 1. Cost Computation
//
void SSCA::CostCompute( CCMethod* ccMtd )
{
	printf( "\n\tCost Computation:" );
	if( ccMtd ) {
		ccMtd->buildCV( lImg, rImg, maxDis, costVol );
#ifdef COMPUTE_RIGHT
		ccMtd->buildRightCV( lImg, rImg, maxDis, rCostVol );
#endif
	} else {
		printf( "\n\t\tDo nothing" );
	}

}
//
// 2. Cost Aggregation
//
void SSCA::CostAggre( CAMethod* caMtd )
{
	printf( "\n\tCost Aggregation:" );
	if( caMtd ) {
		caMtd->aggreCV( lImg, rImg, maxDis, costVol );
#ifdef COMPUTE_RIGHT
		caMtd->aggreCV( rImg, lImg, maxDis, rCostVol );
#endif
	} else {
		printf( "\n\t\tDo nothing" );
	}
	
}
//
// 3. Match
//
void SSCA::Match( void )
{
	printf( "\n\tMatch" );
	for( int y = 0; y < hei; y ++ ) {
		uchar* lDisData = ( uchar* ) lDis.ptr<uchar>( y );
		for( int x = 0; x < wid; x ++ ) {
			double minCost = DOUBLE_MAX;
			int    minDis  = 0;
			for( int d = 1; d < maxDis; d ++ ) {
				double* costData = ( double* )costVol[ d ].ptr<double>( y );
				if( costData[ x ] < minCost ) {
					minCost = costData[ x ];
					minDis  = d;
				}
			}
			lDisData[ x ] = minDis * disSc;
		}
	}
#ifdef COMPUTE_RIGHT
	for( int y = 0; y < hei; y ++ ) {
		uchar* rDisData = ( uchar* ) rDis.ptr<uchar>( y );
		for( int x = 0; x < wid; x ++ ) {
			double minCost = DOUBLE_MAX;
			int    minDis  = 0;
			for( int d = 1; d < maxDis; d ++ ) {
				double* costData = ( double* )rCostVol[ d ].ptr<double>( y );
				if( costData[ x ] < minCost ) {
					minCost = costData[ x ];
					minDis  = d;
				}
			}
			rDisData[ x ] = minDis * disSc;
		}
	}
#endif
}
//
// 4. Post Process;
//
void SSCA::PostProcess( PPMethod* ppMtd )
{
	printf( "\n\tPostProcess:" );
	if( ppMtd ) {
#ifdef COMPUTE_RIGHT
		ppMtd->postProcess( lImg, rImg, maxDis, disSc, lDis, rDis, lSeg, lChk );
#endif
	} else {
		printf( "\n\t\tDo nothing" );
	}
}
#ifdef _DEBUG
// global function to solve all cost volume
void SolveAll( SSCA**& smPyr, const int PY_LVL, const double REG_LAMBDA )
{
	printf( "\n\t\tSolve All" );
	printf( "\n\t\tReg param: %.4lf\n", REG_LAMBDA );
	// construct regularization matrix
	Mat regMat = Mat::zeros( PY_LVL, PY_LVL, CV_64FC1 );
	for( int s = 0; s < PY_LVL; s ++ ) {
		if( s == 0 ) {
			regMat.at<double>( s, s ) = 1 + REG_LAMBDA;
			regMat.at<double>( s, s + 1 ) = - REG_LAMBDA;
		} else if( s == PY_LVL - 1 ) {
			regMat.at<double>( s, s ) = 1 + REG_LAMBDA;
			regMat.at<double>( s, s - 1 ) = - REG_LAMBDA;
		} else {
			regMat.at<double>( s, s ) = 1 + 2 * REG_LAMBDA;
			regMat.at<double>( s, s - 1 ) = - REG_LAMBDA;
			regMat.at<double>( s, s + 1 ) = - REG_LAMBDA;
		}
	}
	Mat regInv = regMat.inv( );
	double* invWgt  = new double[ PY_LVL * PY_LVL ];
	for( int m = 0; m < PY_LVL; m ++ ) {
		double* sWgt = invWgt + m * PY_LVL;
		for( int s = 0; s < PY_LVL; s ++ ) {
			sWgt[ s ] = regInv.at<double>( m, s );
		}
	}

	PrintMat<double>( regInv );

	int hei = smPyr[ 0 ]->hei;
	int wid = smPyr[ 0 ]->wid;

	// backup all cost
/*	Mat** newCosts = new Mat*[ PY_LVL ];
	for( int s = 0; s < PY_LVL; s ++ ) {
		newCosts[ s ] = new Mat[ smPyr[ s ]->maxDis ];
		for( int d = 0; d < smPyr[ s ]->maxDis; d ++ ) {
			newCosts[ s ][ d ] = Mat::zeros( smPyr[ s ]->hei, smPyr[ s ]->wid, CV_64FC1  );
		}
	}*/

	//CPU version
/*	for( int d = 1; d < smPyr[ 0 ]->maxDis; d ++ ) {
		printf( ".s.v." );
		for( int y = 0; y < hei; y ++ ) {
			for( int x = 0; x < wid; x ++ ) {
				for( int m = 0; m < PY_LVL; m ++ ) {
					double sum = 0.0f;
					double* sWgt = invWgt + m * PY_LVL;
					int curY = y;
					int curX = x;
					int curD = d;
					int assY = y;
					int assX = x;
					int assD = d;
					for( int s = 0; s < PY_LVL; s ++ ) {
						if( s == m ) {
							assY = curY;
							assX = curX;
							assD = curD;
						}
						double curCost = smPyr[ s ]->costVol[ curD ].at<double>( curY, curX );
						sum += sWgt[ s ] * curCost;
						curY = curY / 2;
						curX = curX / 2;
						curD = ( curD + 1 ) / 2;
					}
					newCosts[ m ][ assD ].at<double>( assY, assX ) = sum;
				}
			}
		}
	}
*/

	/*
	CUDA实现
	author: cltian
	date: 2018/4/10
	ref: https://hk.saowen.com/a/43287d4469fb06d8e398c2b78572e8165133d687cac9082916ee12c69b44131d
	 */
	//加入新的头文件
	
/*	int imgH = hei;
	int imgW = wid;
	double **pImg = (double**)malloc(sizeof(double*)*PY_LVL*smPyr[ 0 ]->maxDis); //输入 二级指针
	double *pDistData = (double*)malloc(sizeof(double)*PY_LVL*smPyr[ 0 ]->maxDis*imgH*imgW);
	//device 指针
    double **pDevice;//输入 二级指针
    double *pDeviceData;//输入 二级指针
    double **pDstImg;
    double *pDstImgData;//输出图像对应的设备指针
	double *dinvWgt  = new double[ PY_LVL * PY_LVL ];

    cudaCheckError(cudaMalloc(&pDstImgData, sizeof(double)*imgH*imgW*PY_LVL*smPyr[ 0 ]->maxDis));
    cudaCheckError(cudaMalloc(&pDstImg, sizeof(double*)*PY_LVL*smPyr[ 0 ]->maxDis));
    cudaCheckError(cudaMalloc(&pDevice, sizeof(double*)*PY_LVL*smPyr[ 0 ]->maxDis));
    cudaCheckError(cudaMalloc(&pDeviceData, sizeof(double)*imgH*imgW*PY_LVL*smPyr[ 0 ]->maxDis));
	cudaCheckError(cudaMalloc(&dinvWgt, sizeof(double)*PY_LVL*PY_LVL));


    for (int i=0; i<PY_LVL; i++){
    	for (int j=0; j<smPyr[ 0 ]->maxDis; j++){
    		pImg[i] = pDeviceData+imgH*imgW*(i+1)*(j+1);
    	}
    }

    //拷贝到GPU
    cudaCheckError(cudaMemcpy(pDevice, pImg, sizeof(double*)*PY_LVL*smPyr[ 0 ]->maxDis, cudaMemcpyHostToDevice));

    cudaCheckError(cudaMemcpy(dinvWgt, invWgt, sizeof(double)*PY_LVL*PY_LVL, cudaMemcpyHostToDevice));

    for (int i=0; i<PY_LVL; i++){
    	for (int j=0; j<smPyr[ 0 ]->maxDis; j++){
    		cudaCheckError(cudaMemcpy(pDeviceData+imgH*imgW*(i+1)*(j+1), smPyr[i]->costVol[j].data,
    			                      sizeof(double)*imgH*imgW, cudaMemcpyHostToDevice));
    	}
    }
       
    dim3 block(8, 8, 8);
    dim3 grid( (smPyr[ 0 ]->maxDis+block.x-1)/block.x, (imgH+block.y-1)/block.y, (imgW+block.z-1)/block.z);

	solveAllKernel<<<grid, block>>>(pDevice, pDstImg, dinvWgt, imgW, imgH, smPyr[ 0 ]->maxDis, imgH, imgW);

	cudaCheckError(cudaMemcpy(pDistData, pDstImgData, imgW*imgH*PY_LVL*smPyr[ 0 ]->maxDis*sizeof(double), cudaMemcpyDeviceToHost));

	for (int i=0; i<PY_LVL*smPyr[ 0 ]->maxDis; i++)
		cudaCheckError(cudaFree(pDevice[i]));
	cudaCheckError(cudaFree(pDevice));
	cudaCheckError(cudaFree(pDeviceData));
	for (int i=0; i<PY_LVL*smPyr[ 0 ]->maxDis; i++)
		cudaCheckError(cudaFree(pDstImg[i]));
	cudaCheckError(cudaFree(pDstImg));
	cudaCheckError(cudaFree(pDstImgData));
	cudaCheckError(cudaFree(dinvWgt));

	for( int s = 0; s < PY_LVL; s ++ ) {
		for( int d = 0; d < smPyr[ s ]->maxDis; d ++ ) {
			smPyr[ s ]->costVol[ d ] = pDistData[ s*PY_LVL+d ]
			//smPyr[ s ]->costVol[ d ] = newCosts[ s ][ d ].clone();
		}
	}
	free(pDistData);
	for (int i=0; i<PY_LVL*smPyr[ 0 ]->maxDis; i++)
		free(pImg[i]);
	free(pImg);*/

	GpuMat* mats;
	const int constVar1 = PY_LVL*smPyr[ 0 ]->maxDis;
	const int constVar2 = smPyr[ 0 ]->maxDis;
	PtrStepSz<double>* phSrc = new PtrStepSz<double>[constVar1];
	PtrStepSz<double>* phDst = new PtrStepSz<double>[constVar1];
	PtrStepSz<double>* pdSrc;
	PtrStepSz<double>* pdDst;
	double *dinvWgt  = new double[ PY_LVL * PY_LVL 
	int i = 0;
	for(int s=0; s<PY_LVL; s++)
		for(int d=0; constVar2; d++)
			mats[i++].upload(smPyr[s]->constVol[d]);
	for (int i=0; i<constVar1; i++)
		phSrc[i] = mats[i];
    cudaCheckError(cudaMalloc(&dinvWgt, sizeof(double)*PY_LVL*PY_LVL));
	cudaCheckError(cudaMalloc(&pdSrc, constVar1*sizeof(PtrStepSz<double>)));
	cudaCheckError(cudaMalloc(&pdDst, constVar1*sizeof(PtrStepSz<double>)));

	cudaCheckError(cudaMemcpy(pdSrc, phSrc, constVar1*sizeof(PtrStepSz<double>), 
		                      cudaMemcpyHostToDevice));

    dim3 block(8, 8, 8);
    dim3 grid( (constVar2+block.x-1)/block.x, (hei+block.y-1)/block.y, (wid+block.z-1)/block.z);

	solveAllKernel<<<grid, block>>>(pdSrc, pdDst, dinvWgt, constVar2, hei, wid, PY_LVL);

	cudaCheckError(cudaMemcpy(phDst, pdDst, constVar1*sizeof(PtrStepSz<double>)
		                      cudaMemcpyDeviceToHost));
	for( int s = 0; s < PY_LVL; s ++ ) {
		for( int d = 0; d < smPyr[ s ]->maxDis; d ++ ) {
			//smPyr[ s ]->costVol[ d ] = pDistData[ s*PY_LVL+d ]
			Mat tempMatrix;
			phDst[s*PY_LVL+d].download(tempMatrix);
		    smPyr[ s ]->costVol[ d ] = tempMatrix.clone();
	}

	cudaCheckError(cudaFree(pdSrc));
	cudaCheckError(cudaFree(pdDst));
	cudaCheckError(cudaFree(dinvWgt));
/*
end
 */
	// PrintMat<double>( smPyr[ 0 ]->costVol[ 1 ] );
	delete [] invWgt;
/*	for( int s = 0; s < PY_LVL; s ++ ) {
		delete [ ] newCosts[ s ];
	}
	delete [] newCosts;*/
}

void saveOnePixCost( SSCA**& smPyr, const int PY_LVL )
{
	// save as matlab order
	FILE* fp = fopen( "onePix.txt", "w" );
	for( int y = 0; y < smPyr[ 0 ]->hei; y ++ ) {
		for( int x = 0; x < smPyr[ 0 ]->wid; x ++ ) {
			if( y == 24 && x == 443 ) {
				int curY = y;
				int curX = x;
				int prtTime = 1;
				for( int s = 0; s < PY_LVL; s ++ ) {
					fprintf( fp, "(%d,%d): ", curX, curY );
					for( int d = 1; d < smPyr[ s ]->maxDis; d ++ ) {
						double curCost = smPyr[ s ]->costVol[ d ].at<double>( curY, curX );
						for( int p = 0; p < prtTime; p ++ ) {
							fprintf( fp, " %lf", curCost );
						}
					}
					curY = curY / 2;
					curX = curX / 2;
					prtTime *= 2;
					fprintf( fp, "\n" );
				}
			}
		}
	}
	fclose( fp );
}

#else
// global function to solve all cost volume
void SolveAll( SSCA**& smPyr, const int PY_LVL, const double REG_LAMBDA )
{
	printf( "\n\t\tSolve All" );
	printf( "\n\t\tReg param: %.4lf\n", REG_LAMBDA );
	// construct regularization matrix
	Mat regMat = Mat::zeros( PY_LVL, PY_LVL, CV_64FC1 );
	for( int s = 0; s < PY_LVL; s ++ ) {
		if( s == 0 ) {
			regMat.at<double>( s, s ) = 1 + REG_LAMBDA;
			regMat.at<double>( s, s + 1 ) = - REG_LAMBDA;
		} else if( s == PY_LVL - 1 ) {
			regMat.at<double>( s, s ) = 1 + REG_LAMBDA;
			regMat.at<double>( s, s - 1 ) = - REG_LAMBDA;
		} else {
			regMat.at<double>( s, s ) = 1 + 2 * REG_LAMBDA;
			regMat.at<double>( s, s - 1 ) = - REG_LAMBDA;
			regMat.at<double>( s, s + 1 ) = - REG_LAMBDA;
		}
	}
	Mat regInv = regMat.inv( );
	double* invWgt  = new double[ PY_LVL ];
	for( int s = 0; s < PY_LVL; s ++ ) {
		invWgt[ s ] = regInv.at<double>( 0, s );
	}
	PrintMat<double>( regInv );
	int hei = smPyr[ 0 ]->hei;
	int wid = smPyr[ 0 ]->wid;
	// PrintMat<double>( smPyr[ 0 ]->costVol[ 1 ] );
	// for each cost volume divide its mean value
	//for( int s = 0; s < PY_LVL; s++ ) {
	//	for( int y = 0; y < smPyr[ s ]->hei; y ++ ) {
	//		for( int x = 0; x < smPyr[ s ]->wid; x ++ ) {
	//			double meanD = 0;
	//			for( int d = 1; d < smPyr[ s ]->maxDis; d ++ ) {
	//				meanD += smPyr[ s ]->costVol[ d ].at<double>( y, x );
	//			}
	//			meanD /= ( smPyr[ s ]->maxDis - 1 );
	//			if( meanD == 0 ) {
	//				meanD = 1.0;
	//			}
	//			for( int d = 1; d < smPyr[ s ]->maxDis; d ++ ) {
	//				smPyr[ s ]->costVol[ d ].at<double>( y, x ) /= meanD;
	//			}
	//		}
	//	}
	//}
	

	//
	// Left Cost Volume
	//
	for( int d = 1; d < smPyr[ 0 ]->maxDis; d ++ ) {
		// printf( ".s.v." );
		for( int y = 0; y < hei; y ++ ) {
			for( int x = 0; x < wid; x ++ ) {
				int curY = y;
				int curX = x;
				int curD = d;
				double sum = 0;
				for( int s = 0; s < PY_LVL; s ++ ) {
					double curCost = smPyr[ s ]->costVol[ curD ].at<double>( curY, curX );
#ifdef _DEBUG
					if( y == 160 && x == 160 ) {
						printf( "\ns=%d(%d,%d)\td=%d\tcost=%.4lf", s, curY, curX, curD, curCost );
					}
#endif
					sum += invWgt[ s ] * curCost;
					curY = curY / 2;
					curX = curX / 2;
					curD = ( curD + 1 ) / 2;
				}
				smPyr[ 0 ]->costVol[ d ].at<double>( y, x ) = sum;

			}
		}
	}

#ifdef COMPUTE_RIGHT
	//
	// Right Cost Volume
	//
	for( int d = 1; d < smPyr[ 0 ]->maxDis; d ++ ) {
		// printf( ".s.v." );
		for( int y = 0; y < hei; y ++ ) {
			for( int x = 0; x < wid; x ++ ) {
				int curY = y;
				int curX = x;
				int curD = d;
				double sum = 0;
				for( int s = 0; s < PY_LVL; s ++ ) {
					double curCost = smPyr[ s ]->rCostVol[ curD ].at<double>( curY, curX );
#ifdef _DEBUG
					if( y == 160 && x == 160 ) {
						printf( "\ns=%d(%d,%d)\td=%d\tcost=%.4lf", s, curY, curX, curD, curCost );
					}
#endif
					sum += invWgt[ s ] * curCost;
					curY = curY / 2;
					curX = curX / 2;
					curD = ( curD + 1 ) / 2;
				}
				smPyr[ 0 ]->rCostVol[ d ].at<double>( y, x ) = sum;

			}
		}
	}
#endif
	// PrintMat<double>( smPyr[ 0 ]->costVol[ 1 ] );
	delete [] invWgt;
}

#endif

