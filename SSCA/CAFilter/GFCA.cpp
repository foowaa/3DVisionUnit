#include "GFCA.h"
//#include "GuidedFilter.h"
#include "GuidedFilter_cuda.h"

void GFCA::aggreCV( const Mat& lImg, const Mat& rImg, const int maxDis, Mat* costVol )
{
	// filtering cost volume
	for( int d = 1; d < maxDis; d ++ ) {
	     //	printf( "-c-a" );
		GFilter gf(lImg.rows, lImg.cols);
		gf.guidedfilter(costVol[d], lImg, lImg);
		//cv::normalize(costVol[d], costVol[d], 0, 1.0, NORM_MINMAX);
		//costVol[ d ] = GuidedFilter( lImg, costVol[ d ] );
	}
}
