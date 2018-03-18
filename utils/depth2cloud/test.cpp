#include "depth2cloud.h"
//#include "time.h"
//#include <fstream>
//#include <string>
using namespace std;

int main()
{
	//int a = clock();//从这开始计时
	cv::Mat rgb, depth;
	// rgb 图像是8UC3的彩色图像
	rgb = cv::imread("rgb.png");
	// depth 是16UC1的单通道图像，注意flags设置-1,表示读取原始数据不做任何修改
	depth = cv::imread("depth.png", -1);
	int rows = depth.rows;//获取深度图的行数
	int cols = depth.cols;//获取深度图的列数
	PointC * p = new PointC[rows * cols];
	Color * c = new Color[rows * cols];

	//计算三维坐标和RGB颜色值
	CalculatePoint(rgb, depth, p, c);

	//输出为txt文件
	//string FileName("point.txt");//输出文件名

	//ofstream outfile;
	////outfile.open("data.txt", ios::binary | ios::app | ios::in | ios::out);
	//outfile.open(FileName, ios::binary | ios::app | ios::in | ios::out);
	//outfile << "X" << "\t" << "Y" << "\t" << "Z" << "\t" << "R" << "\t" << "G" << "\t" << "B" << "\n";
	//// 把p加入到点云中
	//for (int m = 0; m < rows; m++)
	//	for (int n = 0; n < cols; n++)
	//	{
	//		outfile << (p + m*cols + n)->x << "\t" << (p + m*cols + n)->y << "\t"
	//			<< (p + m*cols + n)->z << "\t" << (c + m*cols + n)->r << "\t"
	//			<< (c + m*cols + n)->g << "\t" << (c + m*cols + n)->b << "\n";
	//	}

	//outfile.close();//关闭文件，保存文件
 //   int b = clock();//到这结束
	//int t = b - a;//算出来的单位是毫秒
	//cout << "time:" << t << "ms" << endl;
	//getchar();
	//清除数据并退出
	rgb.release();
	depth.release();
	delete[]p;
	delete[]c;
	return 0;
	
}
