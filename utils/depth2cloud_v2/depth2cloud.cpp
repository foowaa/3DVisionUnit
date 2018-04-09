# include "depth2cloud.h"


using namespace cv;
using namespace std;
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

			// ��rgbͼ���л�ȡ������ɫ
			// rgb����ͨ����BGR��ʽͼ�����԰������˳���ȡ��ɫ
			/*(c + m*cols + n)->b = rgb.ptr<uchar>(m)[n * 3];
			(c + m*cols + n)->g = rgb.ptr<uchar>(m)[n * 3 + 1];
			(c + m*cols + n)->r = rgb.ptr<uchar>(m)[n * 3 + 2];
			*/

		}
	return;
}
void getFiles(string path, vector<string>& files)
{
	//�ļ����
	long   hFile = 0;
	//�ļ���Ϣ������һ���洢�ļ���Ϣ�Ľṹ��  

	struct _finddata_t fileinfo;

	string p;//�ַ��������·��

	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//�����ҳɹ��������
	{
		do
		{
			//�����Ŀ¼,����֮�����ļ����ڻ����ļ��У�  
			if ((fileinfo.attrib &  _A_SUBDIR))

			{
				//�ļ���������"."&&�ļ���������".."
				//.��ʾ��ǰĿ¼
				//..��ʾ��ǰĿ¼�ĸ�Ŀ¼
				//�ж�ʱ�����߶�Ҫ���ԣ���Ȼ�����޵ݹ�������ȥ�ˣ�			
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			//�������,�����б�  

			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		//_findclose������������
		_findclose(hFile);
	}
}
void Cal_rangesIplImage(char * filePath, IplImage *ranges, IplImage *height)
{
	///ar * filePath = "E:\\��Ŀ\\˫Ŀ�����Ӿ�\\training\\dis";//�Լ�����Ŀ¼  

	vector<string> files;
	////��ȡ��·���µ������ļ�  
	getFiles(filePath, files);
	double rate = 0.5;
	//ranges.convertTo(ranges, CV_32F);
	for (int i = 0; i < files.size(); i++)
	{
		string depthFn = files[i].c_str();
		cv::Mat depth;
		depth = cv::imread(depthFn, -1);


		int rows = depth.rows;//��ȡ���ͼ������
		int cols = depth.cols;//��ȡ���ͼ������
		PointC * p = new PointC[rows * cols];

		//double* data = ranges.ptr<double>(i);
		//double* data1 = height.ptr<double>(i);


		//������ά����
		CalculatePoint(depth, p);

		for (int j = 0; j < rows*cols; j++)
		{
			double xx = (p + j)->x;
			double yy = (p + j)->y;

			CV_IMAGE_ELEM(ranges, double, i, j) = sqrt(xx*xx + yy*yy);
			CV_IMAGE_ELEM(height, double, i, j) = (p + j)->z * rate;

			//data[j] = sqrt(xx*xx + yy*yy);
			//data1[j] = (p + j)->z * rate;

		}
	}
}

void Cal_ranges(char * filePath, Mat &ranges, Mat &height)
{
	//char * filePath = "E:\\��Ŀ\\˫Ŀ�����Ӿ�\\training\\dist";//�Լ�����Ŀ¼  

	vector<string> files;
	////��ȡ��·���µ������ļ�  
	getFiles(filePath, files);
	double rate = 0.5;
	//ranges.convertTo(ranges, CV_32F);
	for (int i = 0; i < files.size(); i++)
	{
		string depthFn = files[i].c_str();
		cv::Mat depth;
		depth = cv::imread(depthFn, -1);

		int rows = depth.rows;//��ȡ���ͼ������
		int cols = depth.cols;//��ȡ���ͼ������
		PointC * p = new PointC[rows * cols];
		double* data = ranges.ptr<double>(i);
		double* data1 = height.ptr<double>(i);
		//������ά����
		CalculatePoint(depth, p);
		for (int j = 0; j < rows*cols; j++)
		{
			double xx = (p + j)->x;
			double yy = (p + j)->y;

			data[j] = sqrt(xx*xx + yy*yy);
			data1[j] = (p + j)->z * rate;

		}
	}

}