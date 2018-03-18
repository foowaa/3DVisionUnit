#include "depth2cloud.h"
//#include "time.h"
using namespace std;

int main()
{
	//int a = clock();//从这开始计时
	cv::Mat depth;
	// rgb 图像是8UC3的彩色图像
	//rgb = cv::imread("rgb.png");
	// depth 是16UC1的单通道图像，注意flags设置-1,表示读取原始数据不做任何修改
	depth = cv::imread("depth.png", -1);
	int rows = depth.rows;//获取深度图的行数
	int cols = depth.cols;//获取深度图的列数
	PointC * p = new PointC[rows * cols];
	/*Color * c = new Color[rows * cols];*/

	//计算三维坐标和RGB颜色值
	CalculatePoint(depth, p);

//	rgb.release();
	depth.release();
	delete[]p;
	//delete[]c;
	return 0;

}
