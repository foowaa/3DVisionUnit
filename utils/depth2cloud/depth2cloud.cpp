# include "depth2cloud.h"

using namespace cv;
// 计算三维坐标及RGB颜色值
void CalculatePoint(Mat depth, PointC *p)
{

	int rows = depth.rows;//获取深度图的行数
	int cols = depth.cols;//获取深度图的列数

	// 遍历深度图
	for (int m = 0; m < rows; m++)
		for (int n = 0; n < cols; n++)
		{
		// 获取深度图中(m,n)处的值
		ushort d = depth.ptr<ushort>(m)[n];
		// d 可能没有值，若如此，跳过此点
		if (d == 0)
			continue;
		// d 存在值，则向点云增加一个点

		// 计算这个点的空间坐标
		(p + m*cols + n)->z = double(d) / camera_factor;
		(p + m*cols + n)->x = (n - camera_cx) * p->z / camera_fx;
		(p + m*cols + n)->y = (m - camera_cy) * p->z / camera_fy;

		//// 从rgb图像中获取它的颜色
		//// rgb是三通道的BGR格式图，所以按下面的顺序获取颜色
		//(c + m*cols + n)->b = rgb.ptr<uchar>(m)[n * 3];
		//(c + m*cols + n)->g = rgb.ptr<uchar>(m)[n * 3 + 1];
		//(c + m*cols + n)->r = rgb.ptr<uchar>(m)[n * 3 + 2];
		}
	return;
}