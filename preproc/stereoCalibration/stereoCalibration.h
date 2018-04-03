#pragma once
//#include "stdafx.h"  
#include <opencv2/opencv.hpp>  
#include <highgui.hpp>  
#include "cv.h"  
#include <cv.hpp>  
#include "stdio.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

void calRealPoint(vector<vector<Point3f>>& obj, int boardwidth, int boardheight, int imgNumber, int squaresize);
void ReadCameraParam(void);
void outputCameraParam(void);
void Cal(string str1, string str2);
void RectifyImage(Mat ImageL, Mat ImageR, Mat &rectifyImageL, Mat &rectifyImageR);

