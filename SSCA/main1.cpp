///////////////////////////////////////////////////////
// File: main.cpp
// Desc: Scale Space Cost Aggregation
// Usage: [CC_METHOD] [CA_METHOD] [USE_MED] [lImg] [rImg] [lDis] [maxDis] [disSc]
// Author: Zhang Kang
// Date: 2013/09/06
///////////////////////////////////////////////////////

#include "CommFunc.h"
#include "SSCA.h"
#include "CC/GrdCC.h"
#include "CAFilter/GFCA.h"
#include "GetMehod.h"
#include "cvtools.h"

#define USE_MEDIAN_FILTER

#ifdef USE_MEDIAN_FILTER
#include"CAST/Toolkit.h"
#endif

int main( int argc, char** argv )
{
	printf( "Scale Space Cost Aggregation\n" );
	// ccName GRD CEN CG 
	// GRD--intensity+gradient; CEN--Census Transform; CG--Census+gradient;
	string ccName = "GRD";
	// caName GF BF BOX NL 
	// GF--guided image filter; BF--bilateral filter; BOX--box filter
	string caName = "GF";
	// ppName WN--weighted median filtering
	string ppName = "WM";
	double costAlpha = atof( "0.3" );
	
	string lFn = "/home/nvidia/project/2/SSCA/data/L.png";
	string rFn = "/home/nvidia/project/2/SSCA/data/R.png";
	string lDisFn = "/home/nvidia/project/2/SSCA/data/dis0001.jpg";

		int maxDis = atoi("256");
		int disSc = atoi("1");
		//
		// Load left right image
		//
		printf("\n--------------------------------------------------------\n");
		//printf( "Load Image: (%s) (%s)\n");
		printf("--------------------------------------------------------\n");
		Mat lImg = imread(lFn, CV_LOAD_IMAGE_COLOR);
		//cv::imshow("lImg", lImg);
		Mat rImg = imread(rFn, CV_LOAD_IMAGE_COLOR);
		if (!lImg.data || !rImg.data) {
			printf("Error: can not open image\n");
			printf("\nPress any key to continue...\n");
			getchar();
			return -1;
		}
		// set image format
		cvtColor(lImg, lImg, CV_BGR2RGB);
		cvtColor(rImg, rImg, CV_BGR2RGB);
		lImg.convertTo(lImg, CV_64F, 1 / 255.0f);
		rImg.convertTo(rImg, CV_64F, 1 / 255.0f);

		// time
		double duration;
		duration = static_cast<double>(getTickCount());

		//
		// Stereo Match at each pyramid
		//
		int PY_LVL = 5;
		// build pyramid and cost volume
		Mat lP = lImg.clone();
		Mat rP = rImg.clone();
		SSCA** smPyr = new SSCA*[PY_LVL];
		CCMethod* ccMtd = getCCType(ccName);
		CAMethod* caMtd = getCAType(caName);
		PPMethod* ppMtd = getPPType(ppName);
		for (int p = 0; p < PY_LVL; p++) {
			if (maxDis < 5) {
				PY_LVL = p;
				break;
			}
			printf("\n\tPyramid: %d:", p);
			smPyr[p] = new SSCA(lP, rP, maxDis, disSc);


			smPyr[p]->CostCompute(ccMtd);

			//if( p== 0 ) {
			//		smPyr[ 0 ]->saveCostVol( "CCGRD.txt" );
			//}
			
			smPyr[p]->CostAggre(caMtd);
			// pyramid downsample
			maxDis = maxDis / 2 + 1;
			disSc *= 2;
			pyrDown(lP, lP);
			pyrDown(rP, rP);
		}
		printf("\n--------------------------------------------------------\n");
		printf("\n Cost Aggregation in Scale Space\n");
		printf("\n--------------------------------------------------------\n");
		// new method
		SolveAll(smPyr, PY_LVL, costAlpha);

		// Match + Postprocess
		//
		smPyr[0]->Match();
		smPyr[0]->PostProcess(ppMtd);
		Mat lDis = smPyr[0]->getLDis();

#ifdef USE_MEDIAN_FILTER
		//
		// Median Filter Output
		//
		MeanFilter(lDis, lDis, 3);
		printf("use median filter");
#endif
		duration = static_cast<double>(getTickCount()) - duration;
		duration /= cv::getTickFrequency(); // the elapsed time in sec
		printf("\n--------------------------------------------------------\n");
		printf("Total Time: %.2lf s\n", duration);
		printf("--------------------------------------------------------\n");

		//
		// Save Output

		imwrite(lDisFn, lDis);
		
		// For drawing - save cost volume
		//
		string costFn = "";
		if (costAlpha > 0.0) {
			costFn = "S_" + caName + ".txt";
		}
		else {
			costFn = caName + ".txt";
		}

		smPyr[0]->saveCostVol(costFn.c_str());

		delete[] smPyr;
		delete ccMtd;
		delete caMtd;
		delete ppMtd;
		return 0;
}

