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
void getfiles(string FileLoadPath, vector<string>& files)
{
    DIR *dir = opendir(FileLoadPath.c_str());
    if (dir == NULL)
    {
        cout << "opendir error" << endl;
        //return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        //if (entry->d_type == 4) continue; //It's dir
        //cout << "name = " << entry->d_name << ", len = " << entry->d_reclen << ", entry->d_type = " << (int)entry->d_type << endl;
        string name = entry->d_name;
        string imgdir = FileLoadPath + name;

        files.push_back(imgdir);
    }
    closedir(dir);
    waitKey(0);
    //system("pause");
    //return 0;
}
/*
void getFiles(const char * dir, vector<string>& files)
{
	intptr_t handle;
	_finddata_t findData;
	string p;

	handle = _findfirst(dir, &findData);    // 查找目录中的第一个文件
	if (handle == -1)
	{
		cout << "Failed to find first file!\n";
		return;
	}

	do
	{
		if (findData.attrib & _A_SUBDIR
			&& strcmp(findData.name, ".") == 0
			&& strcmp(findData.name, "..") == 0
			)    // 是否是子目录并且不为"."或".."
			cout << findData.name << "\t<dir>\n";
		else
		{
			files.push_back(p.assign(dir).append("\\").append(findData.name));
			//cout << findData.name << "\t" << findData.size << endl;
		}
	} while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件

	cout << "Done!\n";
	_findclose(handle);    // 关闭搜索句柄
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
*/

void Cal_rangesIplImage(string gFileLoadPath, IplImage *ranges, IplImage *height)
{
	//char * filePath = "E:\\项目\\双目立体视觉\\training\\dist";//自己设置目录  

	vector<string> files;
	//获取该路径下的所有文件  
	getfiles(gFileLoadPath, files);
	
	double rate = 0.5;
	//ranges.convertTo(ranges, CV_32F);
	Mat r = Mat::zeros(files.size(), 1242 * 375, CV_64FC1);
	Mat h = Mat::zeros(files.size(), 1242 * 375, CV_64FC1);
	for (int i = 0; i < files.size(); i++)
	{
		string depthFn = files[i].c_str();
		//std::cout << files[i].c_str() << std::endl;
		Mat depth;
		depth = imread(depthFn, -1);

		int rows = depth.rows;//获取深度图的行数
		int cols = depth.cols;//获取深度图的列数
		PointC * p = new PointC[rows * cols];

		double* data = r.ptr<double>(i);
		double* data1 = h.ptr<double>(i);

		
		//计算三维坐标
		CalculatePoint(depth, p);

		for (int j = 0; j < rows*cols; j++)
		{
			double xx = (p + j)->x;
			double yy = (p + j)->y;

			//CV_IMAGE_ELEM(ranges, double, i, j) = sqrt(xx*xx + yy*yy);
			//CV_IMAGE_ELEM(height, double, i, j) = (p + j)->z * rate;

			data[j] = sqrt(xx*xx + yy*yy);
			data1[j] = (p + j)->z * rate;

		}
	}
	/*Mat idx = Mat::zeros(r.rows, r.cols, CV_8UC1);
	cv::sortIdx(r, idx, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);*/
	cv::sort(r, r, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);

	/*Mat temp = h;
	for (int i = 0; i < r.rows; i++)
	{
		double* data = h.ptr<double>(i);
		for (int j = 0; j < r.cols; j++)
		{
			int t = idx.ptr<int>(i)[j];
			data[j] = temp.ptr<double>(i)[t];
			
		}
	}*/
	
	for (int i = 0; i < files.size(); i++)
		for (int j = 0; j < 90000;/*1242 * 375; */j++)
		{
			CV_IMAGE_ELEM(ranges, double, i, j) = r.ptr<double>(i)[j];
			CV_IMAGE_ELEM(height, double, i, j) = h.ptr<double>(i)[j];
			/*double elem = r.ptr<double>(i)[j];
			cout << elem << endl;
			getchar();*/
		}
}
/*
void Cal_rangesIplImage(const char * filePath, IplImage *ranges, IplImage *height)
{
//char * filePath = "E:\\项目\\双目立体视觉\\training\\dist";//自己设置目录

vector<string> files;
//获取该路径下的所有文件
getFiles(filePath, files);

double rate = 0.5;
//ranges.convertTo(ranges, CV_32F);
for (int i = 0; i < files.size(); i++)
{
string depthFn = files[i].c_str();
//std::cout << files[i].c_str() << std::endl;
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

}*/
