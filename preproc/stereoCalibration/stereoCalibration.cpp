// stereoCalibration.cpp : �������̨Ӧ�ó������ڵ㡣  
//  
//�ڽ���˫Ŀ����ͷ�ı궨֮ǰ��������ȷֱ����������ͷ���е�Ŀ�Ӿ��ı궨   
//�ֱ�ȷ����������ͷ���ڲξ���Ȼ���ٿ�ʼ����˫Ŀ����ͷ�ı궨  
//�ڴ����������ȶ���������ͷ���е����궨(����һƪ��Ŀ�궨����)��Ȼ���ڽ�������궨  

//#include "stdafx.h"  
#include "stereoCalibration.h"

const int imageWidth = 640;                             //����ͷ�ķֱ���  
const int imageHeight = 480;
const int boardWidth = 9;                               //����Ľǵ���Ŀ  
const int boardHeight = 6;                              //����Ľǵ�����  
const int boardCorner = boardWidth * boardHeight;       //�ܵĽǵ�����  
const int frameNumber = 13;                       //����궨ʱ��Ҫ���õ�ͼ��֡��  
const int squareSize =  20;                              //�궨��ڰ׸��ӵĴ�С ��λmm  
/*
const int imageWidth = 1600;// 640;                             //����ͷ�ķֱ���  
const int imageHeight = 1200;// 480;
const int boardWidth = 11;//9;                               //����Ľǵ���Ŀ  
const int boardHeight = 8;// 6;                              //����Ľǵ�����  
const int boardCorner = boardWidth * boardHeight;       //�ܵĽǵ�����  
const int frameNumber = 19;    //13                         //����궨ʱ��Ҫ���õ�ͼ��֡��  
const int squareSize = 30;// 20;                              //�궨��ڰ׸��ӵĴ�С ��λmm 
*/
const Size boardSize = Size(boardWidth, boardHeight);   //  
Size imageSize = Size(imageWidth, imageHeight);

Mat R, T, E, F;                                         //R ��תʸ�� Tƽ��ʸ�� E�������� F��������  
vector<Mat> rvecs;                                        //��ת����  
vector<Mat> tvecs;                                        //ƽ������  
vector<vector<Point2f>> imagePointL;                    //��������������Ƭ�ǵ�����꼯��  
vector<vector<Point2f>> imagePointR;                    //�ұ������������Ƭ�ǵ�����꼯��  
vector<vector<Point3f>> objRealPoint;                   //����ͼ��Ľǵ��ʵ���������꼯��  


vector<Point2f> cornerL;                              //��������ĳһ��Ƭ�ǵ����꼯��  
vector<Point2f> cornerR;                              //�ұ������ĳһ��Ƭ�ǵ����꼯��  

Mat rgbImageL, grayImageL;
Mat rgbImageR, grayImageR;

Mat Rl, Rr, Pl, Pr, Q;                                  //У����ת����R��ͶӰ����P ��ͶӰ����Q (�����о���ĺ�����ͣ�   
Mat mapLx, mapLy, mapRx, mapRy;                         //ӳ���  
Rect validROIL, validROIR;                              //ͼ��У��֮�󣬻��ͼ����вü��������validROI����ָ�ü�֮�������  

/*
���ȱ궨�õ���������ڲξ���
fx 0 cx
0 fy cy
0 0  1
*/
Mat cameraMatrixL = (Mat_<double>(3, 3) << 532.782, 0, 532.904,
	0, 342.505, 233.876,
	0, 0, 1);
Mat distCoeffL = (Mat_<double>(5, 1) << -0.28095, 0.0255745, 0.00122226, -0.000137736, 0.162946);
/*
���ȱ궨�õ���������ڲξ���
fx 0 cx
0 fy cy
0 0  1
*/
Mat cameraMatrixR = (Mat_<double>(3, 3) << 532.782, 0, 532.904,
	0, 342.505, 233.876,
	0, 0, 1);
Mat distCoeffR = (Mat_<double>(5, 1) << -0.28095, 0.0255745, 0.00122226, -0.000137736, 0.162946);


/*����궨����ģ���ʵ����������*/
void calRealPoint(vector<vector<Point3f>>& obj, int boardwidth, int boardheight, int imgNumber, int squaresize)
{
	//  Mat imgpoint(boardheight, boardwidth, CV_32FC3,Scalar(0,0,0));  
	vector<Point3f> imgpoint;
	for (int rowIndex = 0; rowIndex < boardheight; rowIndex++)
	{
		for (int colIndex = 0; colIndex < boardwidth; colIndex++)
		{
			//  imgpoint.at<Vec3f>(rowIndex, colIndex) = Vec3f(rowIndex * squaresize, colIndex*squaresize, 0);  
			imgpoint.push_back(Point3f(rowIndex * squaresize, colIndex * squaresize, 0));
		}
	}
	for (int imgIndex = 0; imgIndex < imgNumber; imgIndex++)
	{
		obj.push_back(imgpoint);
	}
}
void ReadCameraParam(void)
{
	//��ȡ����
	FileStorage readfs("intrinsics.yml", FileStorage::READ);
	if (readfs.isOpened())
	{
		readfs["cameraMatrixL"] >> cameraMatrixL;
		readfs["cameraDistcoeffL"] >> distCoeffL;
		readfs["cameraMatrixR"] >> cameraMatrixR;
		readfs["cameraDistcoeffR"] >> distCoeffR;
		readfs.release();
	}
	else
	{
		cout << "Error: can not save the intrinsics!!!!!" << endl;
	}

	readfs.open("extrinsics.yml", FileStorage::READ);
	if (readfs.isOpened())
	{
		readfs["R"] >> R;
		readfs["T"] >> T;
		readfs["Rl"] >> Rl;
		readfs["Rr"] >> Rr;
		readfs["Pl"] >> Pl;
		readfs["Pr"] >> Pr;
		readfs["Q"] >> Q;
		readfs.release();
	}
	else
		cout << "Error: can not save the extrinsic parameters\n";
}

void outputCameraParam(void)
{
	//��������
	//�������
	FileStorage fs("intrinsics.yml", FileStorage::WRITE);
	if (fs.isOpened())
	{
		fs << "cameraMatrixL" << cameraMatrixL
			<< "cameraDistcoeffL" << distCoeffL
			<< "cameraMatrixR" << cameraMatrixR
			<< "cameraDistcoeffR" << distCoeffR;
		fs.release();
		//cout << "cameraMatrixL=:" << cameraMatrixL << endl << "cameraDistcoeffL=:" << distCoeffL << endl << "cameraMatrixR=:" << cameraMatrixR << endl << "cameraDistcoeffR=:" << distCoeffR << endl;
	}
	else
	{
		cout << "Error: can not save the intrinsics!!!!!" << endl;
	}

	fs.open("extrinsics.yml", FileStorage::WRITE);
	if (fs.isOpened())
	{
		fs << "R" << R;
		fs << "T" << T;
		fs << "Rl" << Rl;
		fs << "Rr" << Rr;
		fs << "Pl" << Pl;
		fs << "Pr" << Pr;
		fs << "Q" << Q;
		//cout << "R=" << R << endl << "T=" << T << endl << "Rl=" << Rl << endl << "Rr=" << Rr << endl << "Pl=" << Pl << endl << "Pr=" << Pr << endl << "Q=" << Q << endl;
		fs.release();
	}
	else
		cout << "Error: can not save the extrinsic parameters\n";
}



//void Cal(string str1,string str2, Mat &mapLx, Mat &mapLy, Mat &mapRx, Mat &mapRy)
void Cal(string str1, string str2)
{

	ifstream fin1(str1);
	ifstream fin2(str2);
	string filename;
	string filename2;
	while (getline(fin1, filename))
	{
		//char filename[100];
		/*��ȡ��ߵ�ͼ��*/
		//sprintf_s(filename, "left%d.jpg", goodFrameCount + 1);
		rgbImageL = imread(filename, CV_LOAD_IMAGE_COLOR);
		cvtColor(rgbImageL, grayImageL, CV_BGR2GRAY);

		getline(fin2, filename2);
		/*��ȡ�ұߵ�ͼ��*/
		//sprintf_s(filename, "right%d.jpg", goodFrameCount + 1);
		rgbImageR = imread(filename2, CV_LOAD_IMAGE_COLOR);
		cvtColor(rgbImageR, grayImageR, CV_BGR2GRAY);

		bool isFindL, isFindR;

		isFindL = findChessboardCorners(rgbImageL, boardSize, cornerL);
		isFindR = findChessboardCorners(rgbImageR, boardSize, cornerR);
		if (isFindL == true && isFindR == true)  //�������ͼ���ҵ������еĽǵ� ��˵��������ͼ���ǿ��е�  
		{

			cornerSubPix(grayImageL, cornerL, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
			drawChessboardCorners(rgbImageL, boardSize, cornerL, isFindL);
			//imshow("chessboardL", rgbImageL);
			imagePointL.push_back(cornerL);


			cornerSubPix(grayImageR, cornerR, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
			drawChessboardCorners(rgbImageR, boardSize, cornerR, isFindR);
			//imshow("chessboardR", rgbImageR);
			imagePointR.push_back(cornerR);

		}
		else
		{
			cout << "The image is bad please try again" << endl;
		}

		if (waitKey(10) == 'q')
		{
			break;
		}
	}

	/*
	����ʵ�ʵ�У�������ά����
	����ʵ�ʱ궨���ӵĴ�С������
	*/
	calRealPoint(objRealPoint, boardWidth, boardHeight, frameNumber, squareSize);
	cout << "cal real successful" << endl;

	/*
	�궨����ͷ
	��������������ֱ𶼾����˵�Ŀ�궨
	�����ڴ˴�ѡ��flag = CALIB_USE_INTRINSIC_GUESS
	*/
	double rms = stereoCalibrate(objRealPoint, imagePointL, imagePointR,
		cameraMatrixL, distCoeffL,
		cameraMatrixR, distCoeffR,
		Size(imageWidth, imageHeight), R, T, E, F,
		CALIB_USE_INTRINSIC_GUESS,
		TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));

	cout << "Stereo Calibration done with RMS error = " << rms << endl;

	/*
	����У����ʱ����Ҫ����ͼ���沢���ж�׼ ��ʹ������ƥ����ӵĿɿ�
	ʹ������ͼ����ķ������ǰ���������ͷ��ͼ��ͶӰ��һ�������������ϣ�����ÿ��ͼ��ӱ�ͼ��ƽ��ͶӰ������ͼ��ƽ�涼��Ҫһ����ת����R
	stereoRectify �����������ľ��Ǵ�ͼ��ƽ��ͶӰ����������ƽ�����ת����Rl,Rr�� Rl,Rr��Ϊ�������ƽ���ж�׼��У����ת����
	���������Rl��ת�����������Rr��ת֮������ͼ����Ѿ����沢���ж�׼�ˡ�
	����Pl,PrΪ���������ͶӰ�����������ǽ�3D�������ת����ͼ���2D�������:P*[X Y Z 1]' =[x y w]
	Q����Ϊ��ͶӰ���󣬼�����Q���԰�2άƽ��(ͼ��ƽ��)�ϵĵ�ͶӰ��3ά�ռ�ĵ�:Q*[x y d 1] = [X Y Z W]������dΪ��������ͼ���ʱ��
	*/
	stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, imageSize, R, T, Rl, Rr, Pl, Pr, Q,
		CALIB_ZERO_DISPARITY, -1, imageSize, &validROIL, &validROIR);
	/*
	����stereoRectify ���������R �� P ������ͼ���ӳ��� mapx,mapy
	mapx,mapy������ӳ�����������Ը�remap()�������ã���У��ͼ��ʹ������ͼ���沢���ж�׼
	ininUndistortRectifyMap()�Ĳ���newCameraMatrix����У����������������openCV���棬У����ļ��������Mrect�Ǹ�ͶӰ����Pһ�𷵻صġ�
	�������������ﴫ��ͶӰ����P���˺������Դ�ͶӰ����P�ж���У��������������
	*/
	outputCameraParam();

}

void RectifyImage(Mat ImageL, Mat ImageR, Mat &rectifyImageL, Mat &rectifyImageR)
{
	//ReadCameraParam();
	initUndistortRectifyMap(cameraMatrixL, distCoeffL, Rl, Pr, imageSize, CV_32FC1, mapLx, mapLy);
	initUndistortRectifyMap(cameraMatrixR, distCoeffR, Rr, Pr, imageSize, CV_32FC1, mapRx, mapRy);
	/*����stereoRectify ���������R �� P ������ͼ���ӳ��� mapx, mapy
	mapx, mapy������ӳ�����������Ը�remap()�������ã���У��ͼ��ʹ������ͼ���沢���ж�׼
	*/
	remap(ImageL, rectifyImageL, mapLx, mapLy, INTER_LINEAR);
	remap(ImageR, rectifyImageR, mapRx, mapRy, INTER_LINEAR);
}