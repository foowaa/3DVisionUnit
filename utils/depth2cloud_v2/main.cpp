#include "situation.h"
#include "depth2cloud.h"
#include <vector> 
#include <iostream>
#include <fstream> 

int main()
{
	char * filePath ="E:\\项目\\双目立体视觉\\training\\dist";//自己设置深度图的目录
	vector<string> files;
	////获取该路径下的所有文件  
	getFiles(filePath, files);

	int nImageHeightl = 1242 * 375;
	int nImageWidthl = files.size();

	IplImage * ranges = cvCreateImage(cvSize(nImageWidthl, nImageHeightl), IPL_DEPTH_64F, 1);

	IplImage * height = cvCreateImage(cvSize(nImageWidthl, nImageHeightl), IPL_DEPTH_64F, 1);

	Cal_rangesIplImage(filePath, ranges, height);

	//IplImage * ranges = cvCreateImage(cvSize(nImageWidthl, nImageHeightl), IPL_DEPTH_64F, 1);

	//{
	//	ifstream gt;
	//	gt.open("./dad/ranges.txt");

	//	for (int i = 0; i < ranges->height; i++)
	//	{
	//		for (int j = 0; j < ranges->width; j++)
	//		{
	//			gt >> CV_IMAGE_ELEM(ranges, double, i, j);
	//		}
	//	}
	//}


	IplImage * scanAngles = cvCreateImage(cvSize(1, nImageHeightl), IPL_DEPTH_64F, 1);

	{
		ifstream gt;
		gt.open("./dad/scanAngles.txt");

		for (int i = 0; i < scanAngles->height; i++)
		{
			for (int j = 0; j < scanAngles->width; j++)
			{
				gt >> CV_IMAGE_ELEM(scanAngles, double, i, j);
			}
		}
	}

	IplImage * pose = cvCreateImage(cvSize(nImageWidthl, 3), IPL_DEPTH_64F, 1);

	{
		ifstream gt;
		gt.open("./dad/pose.txt");

		for (int i = 0; i < pose->height; i++)
		{
			for (int j = 0; j < pose->width; j++)
			{
				gt >> CV_IMAGE_ELEM(pose, double, i, j);
			}
		}
	}

	//IplImage * height = cvCreateImage(cvSize(nImageWidthl, nImageHeightl), IPL_DEPTH_64F, 1);

	//{
	//	ifstream gt;
	//	gt.open("./dad/height.txt");

	//	for (int i = 0; i < height->height; i++)
	//	{
	//		for (int j = 0; j < height->width; j++)
	//		{
	//			gt >> CV_IMAGE_ELEM(height, double, i, j);
	//		}
	//	}
	//}


	Param param;
	param.origin[0] = 10;
	param.origin[1] = 15;
	param.resol = 50;
	param.size[0] = 35;
	param.size[1] = 50;
	param.lo_occ = 1;
	param.lo_free = 0.5;
	param.lo_max = 100;
	param.lo_min = -100;

	occGridMapping(ranges, scanAngles, pose, height, param);

	cvReleaseImage(&ranges);
	cvReleaseImage(&scanAngles);
	cvReleaseImage(&pose);
	cvReleaseImage(&height);

	return 0;

}