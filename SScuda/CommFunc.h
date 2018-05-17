///////////////////////////////////////////////////////
// File: CommonFunc
// Desc: Common function + Hearder files
//
// Author: Zhang Kang
// Date: 2013/09/06
///////////////////////////////////////////////////////
#pragma  once
#define DOUBLE_MAX 1e10
// #define COMPUTE_RIGHT

#include<opencv2/opencv.hpp>
#include<string>
#include<iostream>
#include<bitset>
#include<stdio.h>
using namespace std;
using namespace cv;

//
// Opencv Lib 2.4.6
//
//#ifdef _DEBUG
//#pragma comment( lib, "opencv_calib3d2411d.lib" )
// //#pragma comment( lib, "opencv_contrib2411d.lib" )
//#pragma comment( lib, "opencv_core2411d.lib" )
//#pragma comment( lib, "opencv_features2d2411d.lib" )
//#pragma comment( lib, "opencv_flann2411d.lib" )
// //#pragma comment( lib, "opencv_gpu2411d.lib" )
//#pragma comment( lib, "opencv_highgui2411d.lib" )
//#pragma comment( lib, "opencv_imgproc2411d.lib" )
// //#pragma comment( lib, "opencv_legacy2411d.lib" )
//#pragma comment( lib, "opencv_ml2411d.lib" )
//#pragma comment( lib, "opencv_nonfree2411d.lib" )
//#pragma comment( lib, "opencv_objdetect2411d.lib" )
//#pragma comment( lib, "opencv_photo2411d.lib" )
//#pragma comment( lib, "opencv_stitching2411d.lib" )
// //#pragma comment( lib, "opencv_superres2411d.lib" )
// //#pragma comment( lib, "opencv_ts2411d.lib" )
////#pragma comment( lib, "opencv_video2411d.lib" )
////#pragma comment( lib, "opencv_videostab2411d.lib" )
//#else
//#pragma comment( lib, "opencv_calib3d2413.lib" )
// //#pragma comment( lib, "opencv_contrib2411.lib" )
//#pragma comment( lib, "opencv_core2413.lib" )
//#pragma comment( lib, "opencv_features2d2413.lib" )
//#pragma comment( lib, "opencv_flann2413.lib" )
////#pragma comment( lib, "opencv_gpu2411.lib" )
//#pragma comment( lib, "opencv_highgui2413.lib" )
//#pragma comment( lib, "opencv_imgproc2413.lib" )
////#pragma comment( lib, "opencv_legacy2411.lib" )
//#pragma comment( lib, "opencv_ml2413.lib" )
//#pragma comment( lib, "opencv_nonfree2413.lib" )
//#pragma comment( lib, "opencv_objdetect2413.lib" )
//#pragma comment( lib, "opencv_photo2413.lib" )
//#pragma comment( lib, "opencv_stitching2413.lib" )
////#pragma comment( lib, "opencv_superres2411.lib" )
////#pragma comment( lib, "opencv_ts2411.lib" )
////#pragma comment( lib, "opencv_video2411.lib" )
////#pragma comment( lib, "opencv_videostab2411.lib" )
//#endif

// output matrix
template<class T>
void PrintMat( const Mat& mat )
{
	int rows = mat.rows;
	int cols = mat.cols;
	printf( "\n%d x %d Matrix\n", rows, cols );
	for( int r = 0; r < rows; r ++ ) {
		for( int c = 0; c < cols; c ++  ) {
			cout << mat.at<T>( r, c ) << "\t";
		}
		printf( "\n" );
	}
	printf( "\n" );
}
