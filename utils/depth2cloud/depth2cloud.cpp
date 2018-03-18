# include "depth2cloud.h"

using namespace cv;
// ������ά���꼰RGB��ɫֵ
void CalculatePoint(Mat depth, PointC *p)
{

	int rows = depth.rows;//��ȡ���ͼ������
	int cols = depth.cols;//��ȡ���ͼ������

	// �������ͼ
	for (int m = 0; m < rows; m++)
		for (int n = 0; n < cols; n++)
		{
		// ��ȡ���ͼ��(m,n)����ֵ
		ushort d = depth.ptr<ushort>(m)[n];
		// d ����û��ֵ������ˣ������˵�
		if (d == 0)
			continue;
		// d ����ֵ�������������һ����

		// ���������Ŀռ�����
		(p + m*cols + n)->z = double(d) / camera_factor;
		(p + m*cols + n)->x = (n - camera_cx) * p->z / camera_fx;
		(p + m*cols + n)->y = (m - camera_cy) * p->z / camera_fy;

		//// ��rgbͼ���л�ȡ������ɫ
		//// rgb����ͨ����BGR��ʽͼ�����԰������˳���ȡ��ɫ
		//(c + m*cols + n)->b = rgb.ptr<uchar>(m)[n * 3];
		//(c + m*cols + n)->g = rgb.ptr<uchar>(m)[n * 3 + 1];
		//(c + m*cols + n)->r = rgb.ptr<uchar>(m)[n * 3 + 2];
		}
	return;
}