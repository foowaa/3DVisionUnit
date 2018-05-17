// stereoCalibration.cpp : 定义控制台应用程序的入口点。  
//  
//在进行双目摄像头的标定之前，最好事先分别对两个摄像头进行单目视觉的标定   
//分别确定两个摄像头的内参矩阵，然后再开始进行双目摄像头的标定  
//在此例程中是先对两个摄像头进行单独标定(见上一篇单目标定文章)，然后在进行立体标定  

//#include "stdafx.h"  
#include "stereoCalibration.h"

const int imageWidth = 640;                             //摄像头的分辨率  
const int imageHeight = 480;
const int boardWidth = 9;                               //横向的角点数目  
const int boardHeight = 6;                              //纵向的角点数据  
const int boardCorner = boardWidth * boardHeight;       //总的角点数据  
const int frameNumber = 13;                       //相机标定时需要采用的图像帧数  
const int squareSize =  20;                              //标定板黑白格子的大小 单位mm  
/*
const int imageWidth = 1600;// 640;                             //摄像头的分辨率  
const int imageHeight = 1200;// 480;
const int boardWidth = 11;//9;                               //横向的角点数目  
const int boardHeight = 8;// 6;                              //纵向的角点数据  
const int boardCorner = boardWidth * boardHeight;       //总的角点数据  
const int frameNumber = 19;    //13                         //相机标定时需要采用的图像帧数  
const int squareSize = 30;// 20;                              //标定板黑白格子的大小 单位mm 
*/
const Size boardSize = Size(boardWidth, boardHeight);   //  
Size imageSize = Size(imageWidth, imageHeight);

Mat R, T, E, F;                                         //R 旋转矢量 T平移矢量 E本征矩阵 F基础矩阵  
vector<Mat> rvecs;                                        //旋转向量  
vector<Mat> tvecs;                                        //平移向量  
vector<vector<Point2f>> imagePointL;                    //左边摄像机所有照片角点的坐标集合  
vector<vector<Point2f>> imagePointR;                    //右边摄像机所有照片角点的坐标集合  
vector<vector<Point3f>> objRealPoint;                   //各副图像的角点的实际物理坐标集合  


vector<Point2f> cornerL;                              //左边摄像机某一照片角点坐标集合  
vector<Point2f> cornerR;                              //右边摄像机某一照片角点坐标集合  

Mat rgbImageL, grayImageL;
Mat rgbImageR, grayImageR;

Mat Rl, Rr, Pl, Pr, Q;                                  //校正旋转矩阵R，投影矩阵P 重投影矩阵Q (下面有具体的含义解释）   
Mat mapLx, mapLy, mapRx, mapRy;                         //映射表  
Rect validROIL, validROIR;                              //图像校正之后，会对图像进行裁剪，这里的validROI就是指裁剪之后的区域  

/*
事先标定好的左相机的内参矩阵
fx 0 cx
0 fy cy
0 0  1
*/
Mat cameraMatrixL = (Mat_<double>(3, 3) << 532.782, 0, 532.904,
	0, 342.505, 233.876,
	0, 0, 1);
Mat distCoeffL = (Mat_<double>(5, 1) << -0.28095, 0.0255745, 0.00122226, -0.000137736, 0.162946);
/*
事先标定好的右相机的内参矩阵
fx 0 cx
0 fy cy
0 0  1
*/
Mat cameraMatrixR = (Mat_<double>(3, 3) << 532.782, 0, 532.904,
	0, 342.505, 233.876,
	0, 0, 1);
Mat distCoeffR = (Mat_<double>(5, 1) << -0.28095, 0.0255745, 0.00122226, -0.000137736, 0.162946);


/*计算标定板上模块的实际物理坐标*/
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
	//读取数据
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
	//保存数据
	//输出数据
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
		/*读取左边的图像*/
		//sprintf_s(filename, "left%d.jpg", goodFrameCount + 1);
		rgbImageL = imread(filename, CV_LOAD_IMAGE_COLOR);
		cvtColor(rgbImageL, grayImageL, CV_BGR2GRAY);

		getline(fin2, filename2);
		/*读取右边的图像*/
		//sprintf_s(filename, "right%d.jpg", goodFrameCount + 1);
		rgbImageR = imread(filename2, CV_LOAD_IMAGE_COLOR);
		cvtColor(rgbImageR, grayImageR, CV_BGR2GRAY);

		bool isFindL, isFindR;

		isFindL = findChessboardCorners(rgbImageL, boardSize, cornerL);
		isFindR = findChessboardCorners(rgbImageR, boardSize, cornerR);
		if (isFindL == true && isFindR == true)  //如果两幅图像都找到了所有的角点 则说明这两幅图像是可行的  
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
	计算实际的校正点的三维坐标
	根据实际标定格子的大小来设置
	*/
	calRealPoint(objRealPoint, boardWidth, boardHeight, frameNumber, squareSize);
	cout << "cal real successful" << endl;

	/*
	标定摄像头
	由于左右摄像机分别都经过了单目标定
	所以在此处选择flag = CALIB_USE_INTRINSIC_GUESS
	*/
	double rms = stereoCalibrate(objRealPoint, imagePointL, imagePointR,
		cameraMatrixL, distCoeffL,
		cameraMatrixR, distCoeffR,
		Size(imageWidth, imageHeight), R, T, E, F,
		CALIB_USE_INTRINSIC_GUESS,
		TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));

	cout << "Stereo Calibration done with RMS error = " << rms << endl;

	/*
	立体校正的时候需要两幅图像共面并且行对准 以使得立体匹配更加的可靠
	使得两幅图像共面的方法就是把两个摄像头的图像投影到一个公共成像面上，这样每幅图像从本图像平面投影到公共图像平面都需要一个旋转矩阵R
	stereoRectify 这个函数计算的就是从图像平面投影都公共成像平面的旋转矩阵Rl,Rr。 Rl,Rr即为左右相机平面行对准的校正旋转矩阵。
	左相机经过Rl旋转，右相机经过Rr旋转之后，两幅图像就已经共面并且行对准了。
	其中Pl,Pr为两个相机的投影矩阵，其作用是将3D点的坐标转换到图像的2D点的坐标:P*[X Y Z 1]' =[x y w]
	Q矩阵为重投影矩阵，即矩阵Q可以把2维平面(图像平面)上的点投影到3维空间的点:Q*[x y d 1] = [X Y Z W]。其中d为左右两幅图像的时差
	*/
	stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, imageSize, R, T, Rl, Rr, Pl, Pr, Q,
		CALIB_ZERO_DISPARITY, -1, imageSize, &validROIL, &validROIR);
	/*
	根据stereoRectify 计算出来的R 和 P 来计算图像的映射表 mapx,mapy
	mapx,mapy这两个映射表接下来可以给remap()函数调用，来校正图像，使得两幅图像共面并且行对准
	ininUndistortRectifyMap()的参数newCameraMatrix就是校正后的摄像机矩阵。在openCV里面，校正后的计算机矩阵Mrect是跟投影矩阵P一起返回的。
	所以我们在这里传入投影矩阵P，此函数可以从投影矩阵P中读出校正后的摄像机矩阵
	*/
	outputCameraParam();

}

void RectifyImage(Mat ImageL, Mat ImageR, Mat &rectifyImageL, Mat &rectifyImageR)
{
	//ReadCameraParam();
	initUndistortRectifyMap(cameraMatrixL, distCoeffL, Rl, Pr, imageSize, CV_32FC1, mapLx, mapLy);
	initUndistortRectifyMap(cameraMatrixR, distCoeffR, Rr, Pr, imageSize, CV_32FC1, mapRx, mapRy);
	/*根据stereoRectify 计算出来的R 和 P 来计算图像的映射表 mapx, mapy
	mapx, mapy这两个映射表接下来可以给remap()函数调用，来校正图像，使得两幅图像共面并且行对准
	*/
	remap(ImageL, rectifyImageL, mapLx, mapLy, INTER_LINEAR);
	remap(ImageR, rectifyImageR, mapRx, mapRy, INTER_LINEAR);
}