
#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
#include <fstream> 
#include <time.h>
using namespace std;

#include "stereoCalibration.h"

#include "CommFunc.h"
#include "SSCA.h"
#include "CC/GrdCC.h"
#include "CAFilter/GFCA.h"
#include "GetMehod.h"
#define USE_MEDIAN_FILTER
#ifdef USE_MEDIAN_FILTER
#include"CAST/Toolkit.h"
#endif

#include "situation.h"
#include "depth2cloud.h"
#include <vector>


//���ⷽλԪ�صĴ洢·��
#define CALIB_EXTRI "/home/nvidia/test/SSCA/Prepro/CalibData/extrinsics.yml"
#define CALIB_INTRI "/home/nvidia/test/SSCA/Prepro/CalibData/intrinsics.yml"
//ͼ�������Ԫ����·��
#define CALIB_META_L "/home/nvidia/test/SSCA/Prepro/CalibData/calibdata_l.txt"
#define CALIB_META_R "/home/nvidia/test/SSCA/Prepro/CalibData/calibdata_r.txt"

//ԭʼ����ͼ���λ��
#define ORIGIN_LEFT_IMG_PATH "/home/nvidia/test/SSCA/Data_img/left1.jpg"
#define ORIGIN_RIGHT_IMG_PATH "/home/nvidia/test/SSCA/Data_img/right1.jpg"
//����������ͼ���λ��
#define CALIBED_LEFT_IMG_PATH "/home/nvidia/test/SSCA/Data_img/left1_calibed.jpg"
#define CALIBED_RIGHT_IMG_PATH "/home/nvidia/test/SSCA/Data_img/right1_calibed.jpg"
//����������ͼ���λ��
#define CALIBED_LEFT_IMG_PATH2 "/home/nvidia/test/SSCA/Data_img/000000_10l.png"
#define CALIBED_RIGHT_IMG_PATH2 "/home/nvidia/test/SSCA/Data_img/000000_10r.png"
//���ͼ���λ��
#define DEPTH_IMG_PATH "/home/nvidia/test/SSCA/Data_depth/dist000000_10.bmp"
//���ͼ��Ŀ¼
#define DEPTH_IMG_FILE "/home/nvidia/test/SSCA/Data_depth/"
//����angle�ļ�
#define CLOUD_ANGLE "/home/nvidia/test/SSCA/Depth2Cloud/dad/scanAngles.txt"
//����pose�ļ�
#define CLOUD_POSE "/home/nvidia/test/SSCA/Depth2Cloud/dad/pose.txt"


//ͼ��Ԥ�������л��߽���
void preProc(String L, String R);
//�������ͼ
void calSSCA(string L, string R, string depthPath);
//��������
void depth2Grid();

#include <fstream>
#include <iostream>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

//int����תstring
void int2str(const int &int_temp, string &string_temp)
{
	stringstream stream;
	stream << int_temp;
	string_temp = stream.str();   //�˴�Ҳ������ stream>>string_temp  
}

int main()
{
	//ͼ����
	int imgCode = 1000;
	//ͼ���ļ���·��
	string imgFilePath = "/home/nvidia/test/SSCA/Data_img/";

	for (int i = 0; i < 1; i++)
	{
		string temp;
		int2str(imgCode, temp);
		string imgLeftPath = imgFilePath + temp + "L.png";
		string imgRightPath = imgFilePath + temp + "R.png";
		string depthPath = DEPTH_IMG_FILE + temp + "Depth.bmp";
		cout << imgLeftPath << endl;
		//��ż�һ
		imgCode++;

		//����ͼ��
		//ͼ��Ԥ�������л��߽���
		preProc(imgLeftPath, imgRightPath);
		//�������ͼ
		calSSCA(imgLeftPath, imgRightPath, depthPath);
		

	}
		
	//��������
	depth2Grid();
	

	//int counter;
	//cin >> counter;
	return 0;
}

//ͼ��Ԥ�������л��߽���
void preProc(String L, String R)
{
	ifstream ifs_ex(CALIB_EXTRI);
	ifstream ifs_in(CALIB_INTRI);
	if (ifs_ex && ifs_in)
	{
		cout << "yes, calib file exis" << endl;
	}
	else
	{
		cout << "no, calib file not exis" << endl;
		string str1 = CALIB_META_L;
		string str2 = CALIB_META_R;
		Cal(str1, str2);
		cout << "calib file success" << endl;
	}
	Mat ImageL = imread(L);
	Mat ImageR = imread(R);
	Mat rectifyImageL;
	Mat rectifyImageR;
	RectifyImage(ImageL, ImageR, rectifyImageL, rectifyImageR);
	//imwrite(L, rectifyImageL);
	//imwrite(R, rectifyImageR);
	ImageL.release();
	ImageR.release();
	rectifyImageL.release();
	rectifyImageR.release();
	cout << "rectify success" << endl;
}

//�������ͼ
void calSSCA(string L, string R, string depthPath)
{
	printf("Scale Space Cost Aggregation\n");
	//if( argc != 10 ) {
	//	printf( "Usage: [CC_METHOD] [CA_METHOD] [PP_METHOD] [C_ALPHA] [lImg] [rImg] [lDis] [maxDis] [disSc]\n" );
	//	printf( "\nPress any key to continue...\n" );
	//	//getchar();
	//	return -1;
	//}
	//string ccName = argv[ 1 ];
	//string caName = argv[ 2 ];
	//string ppName = argv[ 3 ];
	//double costAlpha = atof( argv[ 4 ] );
	//string lFn = argv[ 5 ];
	//string rFn = argv[ 6 ];
	//string lDisFn = argv[ 7 ];
	//int maxDis = atoi( argv[ 8 ] );
	//int disSc  = atoi( argv[ 9 ] );



	int maxDis = atoi("256");
	int disSc = atoi("1");

	string ccName = "GRD";
	string caName = "GF";
	string ppName = "WF";
	double costAlpha = atof("0.3");
	int PY_LVL = 5;
	SSCA** smPyr = new SSCA*[PY_LVL];
	CCMethod* ccMtd = getCCType(ccName);
	CAMethod* caMtd = getCAType(caName);
	PPMethod* ppMtd = getPPType(ppName);


	string lFn = L;
	string rFn = R;
	//string lFn = "E:\\��Ŀ\\˫Ŀ�����Ӿ�\\Rectified1\\left_31.bmp";
	//string rFn = "E:\\��Ŀ\\˫Ŀ�����Ӿ�\\Rectified1\\right_31.bmp";
	string lDisFn = depthPath;

	//
	// Load left right image
	//
	/*printf( "\n--------------------------------------------------------\n" );
	printf( "Load Image: (%s) (%s)\n", argv[ 5 ], argv[ 6 ] );
	printf( "--------------------------------------------------------\n" );*/
	Mat lImg = imread(lFn, CV_LOAD_IMAGE_COLOR);
	Mat rImg = imread(rFn, CV_LOAD_IMAGE_COLOR);

	//if( !lImg.data || !rImg.data ) {
	//	printf( "Error: can not open image\n" );
	//	printf( "\nPress any key to continue...\n" );
	//	//getchar();
	//	return -1;
	//}
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

	// build pyramid and cost volume
	Mat lP = lImg.clone();
	Mat rP = rImg.clone();

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

	// old method
	//for( int p = PY_LVL - 2 ; p >= 0; p -- ) {
	//	smPyr[ p ]->AddPyrCostVol( smPyr[ p + 1 ], costAlpha );
	//}

	//
	// Match + Postprocess
	//
	smPyr[0]->Match();
	smPyr[0]->PostProcess(ppMtd);
	Mat lDis = smPyr[0]->getLDis();

#ifdef _DEBUG
	for (int s = 0; s < PY_LVL; s++) {
		smPyr[s]->Match();
		Mat sDis = smPyr[s]->getLDis();
		ostringstream sStr;
		sStr << s;
		string sFn = sStr.str() + "_ld.png";
		imwrite(sFn, sDis);
	}
	saveOnePixCost(smPyr, PY_LVL);
#endif
#ifdef USE_MEDIAN_FILTER
	//
	// Median Filter Output
	//
	MeanFilter(lDis, lDis, 3);
#endif
	duration = static_cast<double>(getTickCount()) - duration;
	duration /= cv::getTickFrequency(); // the elapsed time in sec
	printf("\n--------------------------------------------------------\n");
	printf("Total Time: %.2lf s\n", duration);
	printf("--------------------------------------------------------\n");

	//
	// Save Output
	//
	imwrite(lDisFn, lDis);

	//
	// For drawing - save cost volume
	//
	//string costFn = "";
	//if( costAlpha > 0.0 ) {
	//	costFn = "S_" + caName + ".txt";
	//} else {
	//	costFn = caName + ".txt";
	//}

	//smPyr[ 0 ]->saveCostVol( costFn.c_str() );



	delete[] smPyr;
	delete ccMtd;
	delete caMtd;
	delete ppMtd;

}

//��������
void depth2Grid()
{
	cout << "show grid:" << endl;

	string gFileLoadPath = "/home/nvidia/test/SSCA/Depth2grid/dist/";//�Լ��������ͼ��Ŀ¼
	vector<string> files;
	//��ȡ��·���µ������ļ�  
	getfiles(gFileLoadPath, files);

	int nImageHeightl = 90000;// 1242 * 375;
	int nImageWidthl = files.size();

	IplImage * ranges = cvCreateImage(cvSize(nImageWidthl, nImageHeightl), IPL_DEPTH_64F, 1);

	IplImage * height = cvCreateImage(cvSize(nImageWidthl, nImageHeightl), IPL_DEPTH_64F, 1);

	Cal_rangesIplImage(gFileLoadPath, ranges, height);

	/*int nImageHeightl = 2000;
	int nImageWidthl = 77;
	IplImage * ranges = cvCreateImage(cvSize(nImageWidthl, nImageHeightl), IPL_DEPTH_64F, 1);

	{
		ifstream gt;
		gt.open("./dad/ranges.txt");

		for (int i = 0; i < ranges->height; i++)
		{
			for (int j = 0; j < ranges->width; j++)
			{
				gt >> CV_IMAGE_ELEM(ranges, double, i, j);
			}
		}
	}*/


	IplImage * scanAngles = cvCreateImage(cvSize(1, nImageHeightl), IPL_DEPTH_64F, 1);

	{
		ifstream gt;
		gt.open("/home/nvidia/test/SSCA/Depth2grid/dad/scanAngles.txt");

		for (int i = 0; i < scanAngles->height; i++)
		{
			for (int j = 0; j < scanAngles->width; j++)
			{
				gt >> CV_IMAGE_ELEM(scanAngles, double, i, j);
			}
		}
	}

	IplImage * pose = cvCreateImage(cvSize(77, 3), IPL_DEPTH_64F, 1);

	{
		ifstream gt;
		gt.open("/home/nvidia/test/SSCA/Depth2grid/dad/pose.txt");

		for (int i = 0; i < pose->height; i++)
		{
			for (int j = 0; j < pose->width; j++)
			{
				gt >> CV_IMAGE_ELEM(pose, double, i, j);
				//std::cout << CV_IMAGE_ELEM(pose, double, i, j) << std::endl;
			}
		}
	}

	/*IplImage * height = cvCreateImage(cvSize(nImageWidthl, nImageHeightl), IPL_DEPTH_64F, 1);

	{
		ifstream gt;
		gt.open("./dad/height.txt");

		for (int i = 0; i < height->height; i++)
		{
			for (int j = 0; j < height->width; j++)
			{
				gt >> CV_IMAGE_ELEM(height, double, i, j);
			}
		}
	}*/
	
	

	Paramm param;
	param.origin[0] = 10;//10;
	param.origin[1] = 15;//15;
	param.resol = 50;
	param.size[0] = 50;//35;
	param.size[1] = 100;// 50;
	param.lo_occ = 1;
	param.lo_free = 0.5;
	param.lo_max = 100;
	param.lo_min = -100;

	//occGridMapping(ranges, scanAngles, pose, height, param);
	clock_t start, finish;
	double duration;
	start = clock();

	occGridMapping(ranges, scanAngles, pose, param);

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	std::cout << duration << std::endl;

	cvReleaseImage(&ranges);
	cvReleaseImage(&scanAngles);
	cvReleaseImage(&pose);
	cvReleaseImage(&height);

	/*finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	std::cout << duration << std::endl;
	getchar(); */

	//return 0;

}
