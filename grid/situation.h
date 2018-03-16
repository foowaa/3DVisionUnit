#include <windows.h>
#include <vector> 
#include <fstream>
#include<opencv/cv.h>               
#include<opencv/cvaux.h>
#include<opencv/highgui.h>
#include<opencv/cxcore.h>  


using namespace std;


typedef vector<int> my_int;

struct Param
{
	int resol;
	int size[2];
	double origin[2];
	double lo_occ;
	double lo_free;
	double lo_max;
	double lo_min;
};

int mod(double x,double y);
void bresenham1(vector<int> &x,vector<int> &y,double x1,double y1, double x2,double y2);
IplImage * grids(IplImage *range,IplImage *scanAngle,IplImage *pose,double *origin,double resolution,int WQ);
void occGridMapping(IplImage *ranges,IplImage * scanAngles, IplImage *pose,IplImage * height, Param param);