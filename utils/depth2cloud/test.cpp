#include "depth2cloud.h"
//#include "time.h"
using namespace std;

int main()
{
	//int a = clock();//���⿪ʼ��ʱ
	cv::Mat depth;
	// rgb ͼ����8UC3�Ĳ�ɫͼ��
	//rgb = cv::imread("rgb.png");
	// depth ��16UC1�ĵ�ͨ��ͼ��ע��flags����-1,��ʾ��ȡԭʼ���ݲ����κ��޸�
	depth = cv::imread("depth.png", -1);
	int rows = depth.rows;//��ȡ���ͼ������
	int cols = depth.cols;//��ȡ���ͼ������
	PointC * p = new PointC[rows * cols];
	/*Color * c = new Color[rows * cols];*/

	//������ά�����RGB��ɫֵ
	CalculatePoint(depth, p);

//	rgb.release();
	depth.release();
	delete[]p;
	//delete[]c;
	return 0;

}
