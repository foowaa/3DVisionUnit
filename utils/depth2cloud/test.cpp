#include "depth2cloud.h"
//#include "time.h"
//#include <fstream>
//#include <string>
using namespace std;

int main()
{
	//int a = clock();//���⿪ʼ��ʱ
	cv::Mat rgb, depth;
	// rgb ͼ����8UC3�Ĳ�ɫͼ��
	rgb = cv::imread("rgb.png");
	// depth ��16UC1�ĵ�ͨ��ͼ��ע��flags����-1,��ʾ��ȡԭʼ���ݲ����κ��޸�
	depth = cv::imread("depth.png", -1);
	int rows = depth.rows;//��ȡ���ͼ������
	int cols = depth.cols;//��ȡ���ͼ������
	PointC * p = new PointC[rows * cols];
	Color * c = new Color[rows * cols];

	//������ά�����RGB��ɫֵ
	CalculatePoint(rgb, depth, p, c);

	//���Ϊtxt�ļ�
	//string FileName("point.txt");//����ļ���

	//ofstream outfile;
	////outfile.open("data.txt", ios::binary | ios::app | ios::in | ios::out);
	//outfile.open(FileName, ios::binary | ios::app | ios::in | ios::out);
	//outfile << "X" << "\t" << "Y" << "\t" << "Z" << "\t" << "R" << "\t" << "G" << "\t" << "B" << "\n";
	//// ��p���뵽������
	//for (int m = 0; m < rows; m++)
	//	for (int n = 0; n < cols; n++)
	//	{
	//		outfile << (p + m*cols + n)->x << "\t" << (p + m*cols + n)->y << "\t"
	//			<< (p + m*cols + n)->z << "\t" << (c + m*cols + n)->r << "\t"
	//			<< (c + m*cols + n)->g << "\t" << (c + m*cols + n)->b << "\n";
	//	}

	//outfile.close();//�ر��ļ��������ļ�
 //   int b = clock();//�������
	//int t = b - a;//������ĵ�λ�Ǻ���
	//cout << "time:" << t << "ms" << endl;
	//getchar();
	//������ݲ��˳�
	rgb.release();
	depth.release();
	delete[]p;
	delete[]c;
	return 0;
	
}
