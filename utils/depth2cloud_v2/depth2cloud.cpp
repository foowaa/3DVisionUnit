# include "depth2cloud.h"


using namespace cv;
using namespace std;
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

			// 从rgb图像中获取它的颜色
			// rgb是三通道的BGR格式图，所以按下面的顺序获取颜色
			/*(c + m*cols + n)->b = rgb.ptr<uchar>(m)[n * 3];
			(c + m*cols + n)->g = rgb.ptr<uchar>(m)[n * 3 + 1];
			(c + m*cols + n)->r = rgb.ptr<uchar>(m)[n * 3 + 2];
			*/

		}
	return;
}
void getFiles(string path, vector<string>& files)
{
	//文件句柄
	long   hFile = 0;
	//文件信息，声明一个存储文件信息的结构体  

	struct _finddata_t fileinfo;

	string p;//字符串，存放路径

	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//若查找成功，则进入
	{
		do
		{
			//如果是目录,迭代之（即文件夹内还有文件夹）  
			if ((fileinfo.attrib &  _A_SUBDIR))

			{
				//文件名不等于"."&&文件名不等于".."
				//.表示当前目录
				//..表示当前目录的父目录
				//判断时，两者都要忽略，不然就无限递归跳不出去了！			
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			//如果不是,加入列表  

			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		//_findclose函数结束查找
		_findclose(hFile);
	}
}
void Cal_rangesIplImage(char * filePath, IplImage *ranges, IplImage *height)
{
	///ar * filePath = "E:\\项目\\双目立体视觉\\training\\dis";//自己设置目录  

	vector<string> files;
	////获取该路径下的所有文件  
	getFiles(filePath, files);
	double rate = 0.5;
	//ranges.convertTo(ranges, CV_32F);
	for (int i = 0; i < files.size(); i++)
	{
		string depthFn = files[i].c_str();
		cv::Mat depth;
		depth = cv::imread(depthFn, -1);


		int rows = depth.rows;//获取深度图的行数
		int cols = depth.cols;//获取深度图的列数
		PointC * p = new PointC[rows * cols];

		//double* data = ranges.ptr<double>(i);
		//double* data1 = height.ptr<double>(i);


		//计算三维坐标
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
	//char * filePath = "E:\\项目\\双目立体视觉\\training\\dist";//自己设置目录  

	vector<string> files;
	////获取该路径下的所有文件  
	getFiles(filePath, files);
	double rate = 0.5;
	//ranges.convertTo(ranges, CV_32F);
	for (int i = 0; i < files.size(); i++)
	{
		string depthFn = files[i].c_str();
		cv::Mat depth;
		depth = cv::imread(depthFn, -1);

		int rows = depth.rows;//获取深度图的行数
		int cols = depth.cols;//获取深度图的列数
		PointC * p = new PointC[rows * cols];
		double* data = ranges.ptr<double>(i);
		double* data1 = height.ptr<double>(i);
		//计算三维坐标
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