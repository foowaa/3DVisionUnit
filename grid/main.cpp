#include "situation.h"

#include <windows.h>
#include <vector> 
#include <fstream> 


void main()   
{   
	int nImageHeightl=2000;
	int nImageWidthl= 77 ;

	IplImage * ranges = cvCreateImage(cvSize(nImageWidthl,nImageHeightl),IPL_DEPTH_64F,1);

	{
		ifstream gt;
		gt.open("E:\\WORKING\\dad\\ranges.txt");

		for (int i = 0;i<ranges->height;i++)
		{
			for(int j = 0;j<ranges->width;j++)
			{
				gt>>CV_IMAGE_ELEM(ranges,double,i,j);
			}
		}
	}

	IplImage * scanAngles = cvCreateImage(cvSize(1,nImageHeightl),IPL_DEPTH_64F,1);

	{
		ifstream gt;
		gt.open("E:\\WORKING\\dad\\scanAngles.txt");

		for (int i = 0;i<scanAngles->height;i++)
		{
			for(int j = 0;j<scanAngles->width;j++)
			{
				gt>>CV_IMAGE_ELEM(scanAngles,double,i,j);
			}
		}
	}

	IplImage * pose = cvCreateImage(cvSize(nImageWidthl,3),IPL_DEPTH_64F,1);

	{
		ifstream gt;
		gt.open("E:\\WORKING\\dad\\pose.txt");

		for (int i = 0;i<pose->height;i++)
		{
			for(int j = 0;j<pose->width;j++)
			{
				gt>>CV_IMAGE_ELEM(pose,double,i,j);
			}
		}
	}

	IplImage * height = cvCreateImage(cvSize(nImageWidthl,nImageHeightl),IPL_DEPTH_64F,1);

	{
		ifstream gt;
		gt.open("E:\\WORKING\\dad\\height.txt");

		for (int i = 0;i<height->height;i++)
		{
			for(int j = 0;j<height->width;j++)
			{
				gt>>CV_IMAGE_ELEM(height,double,i,j);
			}
		}
	}

	Param param;
	param.origin[0]=10;
	param.origin[1]=15;
	param.resol = 50;
	param.size[0] = 35;
	param.size[1] = 50;
	param.lo_occ = 1;
	param.lo_free = 0.5;
	param.lo_max =100;
	param.lo_min =-100;

	occGridMapping(ranges, scanAngles, pose, height, param);

	cvReleaseImage(&ranges);
	cvReleaseImage(&scanAngles);
	cvReleaseImage(&pose);
	cvReleaseImage(&height);
}