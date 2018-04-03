#include "stereoCalibration.h"

int main()
{
	//string str1 = "Data/calibdata_l.txt"; //标定所用图像文件的路径
	//string str2 = "Data/calibdata_r.txt";	
	//Cal(str1, str2);

	Mat ImageL = imread("Data/left1.jpg");
	Mat ImageR = imread("Data/right1.jpg");
	Mat rectifyImageL;
	Mat rectifyImageR;
	RectifyImage(ImageL, ImageR, rectifyImageL, rectifyImageR);
	imwrite("Data/R_imgL.jpg", rectifyImageL);
	imwrite("Data/R_imgR.jpg", rectifyImageR);
	ImageL.release();
	ImageR.release();
	rectifyImageL.release();
	rectifyImageR.release();
	return 0;
}