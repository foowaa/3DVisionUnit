#include "situation.h"

int mod(double x,double y)
{
	double n=floor(x/y);
	int res=x-n*y;
	return res;
}
void bresenham1(vector<int> &x,vector<int> &y,double x1,double y1, double x2,double y2)
{
	//vector<double> x;
	//vector<double> y;
	x.clear();
	y.clear();

	x1=int(x1+0.5); 
	x2=int(x2+0.5);

	y1=int(y1+0.5); 
	y2=int(y2+0.5); 

	double dx=abs(x2-x1);
	double dy=abs(y2-y1);

	double steep=abs(dy)>abs(dx);

	if(steep!=0)
	{
		double t=dx;
		dx=dy;
		dy=t; 
	}

	IplImage *q;
	if (dy==0) 
	{
		q=cvCreateImage(cvSize(1,dx+1),IPL_DEPTH_64F,1);
		cvZero(q);
	}
	else
	{
		vector<int> Po;
		Po.clear();
		double d=-dy*dx+floor(dx/2);

		for(int i=floor(dx/2);i>=-dy*dx+floor(dx/2);)
		{
			Po.push_back(i);
			i=i-dy;
		}

		q=cvCreateImage(cvSize(1,Po.size()),IPL_DEPTH_64F,1);

		CV_IMAGE_ELEM(q,double,0,0)=0;

		for(int i=0;i<Po.size()-1;i++)
		{
			double d=(mod(Po[i+1],dx)-mod(Po[i],dx));
			if(d>=0)
				CV_IMAGE_ELEM(q,double,0,i+1)=1;
			else
				CV_IMAGE_ELEM(q,double,0,i+1)=0;
		}
		//{
		//	char *filenamejpg="F:\\q.txt";
		//	ofstream os33;
		//	os33.open(filenamejpg);
		//	for(int i=0;i<q->height;i++)
		//	{
		//		for(int j=0;j<q->width;j++)
		//		{
		//			//double cd=(uchar)CV_IMAGE_ELEM(index_im,double,i,j);
		//                 double cd=CV_IMAGE_ELEM(q,double,i,j);;
		//			
		//			//double cd= (uchar)im->imageData[(i)*im->widthStep+(j)*3];
		//			
		//			os33<<cd<<",";
		//		}
		//		os33<<endl;
		//	}
		//	os33.close();
		//}
	}

	if(steep!=0)
	{
		if(y1<=y2)
		{
			for(int i=y1;i<=y2;i++)
			{
				y.push_back(i);
			}
		}
		else
		{
			for(int i=y1;i>=y2;i--)
			{
				y.push_back(i);
			}
		}

		if(x1<=x2)
		{
			double xin=0;
			for(int i=0;i<q->height;i++)
			{
				xin=CV_IMAGE_ELEM(q,double,i,0)+xin;
				x.push_back(x1+xin);
			}
		}
		else
		{
			double xin=0;
			for(int i=0;i<q->height;i++)
			{
				xin=CV_IMAGE_ELEM(q,double,i,0)+xin;
				x.push_back(x1-xin);
			}
		}
	}
	else
	{
		if(x1<=x2)
		{
			for(int i=x1;i<=x2;i++)
			{
				x.push_back(i);
			}
		}
		else
		{
			for(int i=x1;i>=x2;i--)
			{
				x.push_back(i);
			}
		}

		if(y1<=y2)
		{
			double yin=0;
			for(int i=0;i<q->height;i++)
			{
				yin=CV_IMAGE_ELEM(q,double,i,0)+yin;
				y.push_back(y1+yin);
			}
		}
		else
		{
			double yin=0;
			for(int i=0;i<q->height;i++)
			{
				yin=CV_IMAGE_ELEM(q,double,i,0)+yin;
				y.push_back(y1-yin);
			}
		}
	}

	cvReleaseImage(&q);
}


IplImage * grids(IplImage *range,IplImage *scanAngle,IplImage *pose,double *origin,double resolution,int WQ)
{
	IplImage *gridCord = cvCreateImage(cvSize(2,scanAngle->height),IPL_DEPTH_64F,1);

	double pose0 = CV_IMAGE_ELEM(pose,double,0,WQ);
	double pose1 = CV_IMAGE_ELEM(pose,double,1,WQ);
	double pose2 = CV_IMAGE_ELEM(pose,double,2,WQ);

	double origin0=origin[0];
	double origin1=origin[1];

	//{
	//	char *filenamejpg="F:\\scanAngle.txt";
	//	ofstream os33;
	//	os33.open(filenamejpg);
	//	for(int i=0;i<scanAngle->height;i++)
	//	{
	//		for(int j=0;j<scanAngle->width;j++)
	//		{
	//			//double cd=(uchar)CV_IMAGE_ELEM(index_im,double,i,j);
	//			double cd=CV_IMAGE_ELEM(scanAngle,double,i,j);;
	//			//double cd= (uchar)im->imageData[(i)*im->widthStep+(j)*3];
	//			os33<<cd<<",";
	//		}
	//		os33<<endl;
	//	}
	//	os33.close();
	//}

	int j=0;
	for (int i = 0;i<gridCord->height;i++)
	{
		//double x_occ = 0;
		double x_occ = CV_IMAGE_ELEM(range,double,i,WQ)*cos(CV_IMAGE_ELEM(scanAngle,double,i,0)+pose2)+pose0;
		//double y_occ = -CV_IMAGE_ELEM(range,double,i,WQ)*sin(CV_IMAGE_ELEM(pose,double,WQ,2))+CV_IMAGE_ELEM(pose,double,WQ,1);
		//double  df1 = ceil(x_occ*resolution)+origin[j];
		//double  df2 = ceil(y_occ*resolution)+origin[j];

		//if(j=0)
		CV_IMAGE_ELEM(gridCord,double,i,j)=ceil(x_occ*resolution)+origin[j];
		//if(j==1)
		//CV_IMAGE_ELEM(gridCord,double,i,j)=ceil(y_occ*resolution)+origin[j];
	}

	j=1;
	for (int i = 0;i<gridCord->height;i++)
	{
		//double x_occ = CV_IMAGE_ELEM(range,double,i,WQ)*cos(CV_IMAGE_ELEM(pose,double,WQ,2))+CV_IMAGE_ELEM(pose,double,WQ,0);
		//double y_occ = 0;
		double y_occ = -CV_IMAGE_ELEM(range,double,i,WQ)*sin(CV_IMAGE_ELEM(scanAngle,double,i,0)+pose2)+pose1;

		//double  df1 = ceil(x_occ*resolution)+origin[j];
		//double  df2 = ceil(y_occ*resolution)+origin[j];

		//if(j=0)
		//	CV_IMAGE_ELEM(gridCord,double,i,j)=ceil(x_occ*resolution)+origin[j];
		//if(j==1)
		CV_IMAGE_ELEM(gridCord,double,i,j)=ceil(y_occ*resolution)+origin[j];
	}
	return gridCord;
}

void occGridMapping(IplImage *ranges, IplImage * scanAngles, IplImage *pose, Param param)
//void occGridMapping(IplImage *ranges,IplImage * scanAngles, IplImage *pose,IplImage * height, Param param)
{
	int myResol = param.resol;
	IplImage * myMap = cvCreateImage(cvSize(param.size[1],param.size[0]),IPL_DEPTH_64F,1);
	cvZero(myMap);

	IplImage * CX = cvCreateImage(cvSize(param.size[1],param.size[0]),8,1);

	IplImage * CXRGB = cvCreateImage(cvSize(param.size[1],param.size[0]),8,3);

	IplImage * CX1 = cvCreateImage(cvSize(param.size[1]*16,param.size[0]*16),8,3);

	int myOrigin[2];
	myOrigin[0]= param.origin[0];
	myOrigin[1]= param.origin[1];

	double lo_occ = param.lo_occ;
	double lo_free = param.lo_free; 
	double lo_max = param.lo_max;
	double lo_min = param.lo_min;

	int N = pose->width;

	double * SD =new double [2];
	SD[0]=param.origin[0];
	SD[1]=param.origin[1];

	//{
	//	char *filenamejpg="F:\\1.txt";
	//	ofstream os33;
	//	os33.open(filenamejpg);
	//	for(int i=0;i<ranges->height;i++)
	//	{
	//		for(int j=0;j<ranges->width;j++)
	//		{
	//			//double cd=(uchar)CV_IMAGE_ELEM(index_im,double,i,j);
	//			double cd=CV_IMAGE_ELEM(ranges,double,i,j);;

	//			//double cd= (uchar)im->imageData[(i)*im->widthStep+(j)*3];

	//			os33<<cd<<" ";
	//		}
	//		os33<<endl;
	//	}
	//	os33.close();
	//}

	vector<int> gridPosex;
	vector<int> gridPosey;
	gridPosex.clear();
	gridPosey.clear();

	for(int j=0;j<N;j++)
	{ 
		double ds=CV_IMAGE_ELEM(pose,double,0,j);
		double ds1=CV_IMAGE_ELEM(pose,double,1,j);
		int gridPose[2];
		gridPose[0] = ceil(myResol*CV_IMAGE_ELEM(pose,double,0,j)) + myOrigin[0];
		gridPose[1] = ceil(myResol*CV_IMAGE_ELEM(pose,double,1,j)) + myOrigin[1];

		gridPosex.push_back(gridPose[0]);
		gridPosey.push_back(gridPose[1]);

		IplImage* occCells =grids(ranges,scanAngles,pose,SD,50,j);

		for(int i=0;i<occCells->height;i++)
		{
			vector<int> x;
			vector<int> y;

			int occCells1=CV_IMAGE_ELEM(occCells,double,i,0);
			int occCells2=CV_IMAGE_ELEM(occCells,double,i,1);

			if (occCells1 >= 1 && occCells2 >= 1)
			{
				bresenham1(x, y, gridPose[0], gridPose[1], occCells1, occCells2);

				if (occCells1 >= 1 && occCells2 >= 1)
					CV_IMAGE_ELEM(myMap, double, occCells2 - 1, occCells1 - 1) = CV_IMAGE_ELEM(myMap, double, occCells2 - 1, occCells1 - 1) + (int)lo_occ;

				for (int ii = 0; ii<x.size(); ii++)
				{
					if (y[ii] >= 1 && x[ii] >= 1)
						CV_IMAGE_ELEM(myMap, double, y[ii] - 1, x[ii] - 1) = CV_IMAGE_ELEM(myMap, double, y[ii] - 1, x[ii] - 1) - lo_free;
				}
			}

		}

		//{
		//	char *filenamejpg="F:\\Q1.txt";
		//	ofstream os33;
		//	os33.open(filenamejpg);
		//	for(int i=0;i<myMap->height;i++)
		//	{
		//		for(int j=0;j<myMap->width;j++)
		//		{
		//			//double cd=(uchar)CV_IMAGE_ELEM(index_im,double,i,j);
		//			double cd= CV_IMAGE_ELEM(myMap,double,i,j);
		//			//double cd= (uchar)im->imageData[(i)*im->widthStep+(j)*3];
		//			os33<<cd<<",";
		//		}
		//		os33<<endl;
		//	}
		//	os33.close();
		//}

		for(int i=0;i<myMap->height;i++)
		{
			for(int j=0;j<myMap->width;j++)
			{
				if(CV_IMAGE_ELEM(myMap,double,i,j)>lo_max)
					CV_IMAGE_ELEM(myMap,double,i,j)=lo_max;
				if(CV_IMAGE_ELEM(myMap,double,i,j)<lo_min)
					CV_IMAGE_ELEM(myMap,double,i,j)=lo_min;		
			}
		}

		double myMapmax=-1000;
		double myMapmin=1000; 
		for(int i=0;i<myMap->height;i++)
		{
			for(int j=0;j<myMap->width;j++)
			{
				if(CV_IMAGE_ELEM(myMap,double,i,j)>myMapmax)
					myMapmax=CV_IMAGE_ELEM(myMap,double,i,j);
				if(CV_IMAGE_ELEM(myMap,double,i,j)<myMapmin)
					myMapmin=CV_IMAGE_ELEM(myMap,double,i,j);		
			}
		}

		for(int i=0;i<myMap->height;i++)
		{
			for(int j=0;j<myMap->width;j++)
			{
				CV_IMAGE_ELEM(CX,byte,i,j)=(myMapmax-CV_IMAGE_ELEM(myMap,double,i,j)+myMapmin);
				//255-(byte)((myMapmax-myMapmin)/255*(CV_IMAGE_ELEM(myMap,double,i,j)-myMapmin));
			}
		}

		cvCvtColor(CX,CXRGB, CV_GRAY2RGB);

		//IplImage* pBinary=cvLoadImage("c://temp.jpg",0);  
		//Mat Img;  
		//Img=cvarrToMat(CXRGB); 


		for(int i=0;i<gridPosex.size()-1;i++)
		{
			CvPoint start;
			start.x=gridPosex[i];
			start.y=gridPosey[i]+10;

			CvPoint end;
			end.x=gridPosex[i+1];
			end.y=gridPosey[i+1]+10;

			cvLine(CXRGB,start,end,CV_RGB(255,0,0),0.2,8,0);
			//cv::line(Img, start, end, cv::Scalar(0, 255, 255));  
		}


		//IplImage* pBinary = &IplImage(Img);  
		cvResize(CXRGB,CX1);
		//for (int i = 0; i<gridPosex.size() - 1; i++)
		//{
		//	CvPoint start;
		//	start.x = gridPosex[i]*16;
		//	start.y = (gridPosey[i] )*16-3;

		//	CvPoint end;
		//	end.x = gridPosex[i + 1]*16;
		//	end.y = (gridPosey[i + 1] )*16-3;

		//	cvLine(CX1, start, end, CV_RGB(255, 0, 0), 4, 8, 0);
		//	//cv::line(Img, start, end, cv::Scalar(0, 255, 255));  
		//}
		//		cvShowImage("1",imgDouble);
		cvShowImage("3", CX1);

		cvWaitKey(300);


		//while(1)
		//{
		//	if(cvWaitKey(100)==27)
		//		break;
		//}
		cvReleaseImage(&occCells);

		//	{
		//		char *filenamejpg="F:\\myMap.txt";
		//		ofstream os33;
		//		os33.open(filenamejpg);
		//		for(int i=0;i<myMap->height;i++)
		//		{
		//			for(int j=0;j<myMap->width;j++)
		//			{
		//				//double cd=(uchar)CV_IMAGE_ELEM(index_im,double,i,j);
		//				double cd= CV_IMAGE_ELEM(myMap,double,i,j);

		//				//double cd= (uchar)im->imageData[(i)*im->widthStep+(j)*3];

		//				os33<<cd<<",";
		//			}
		//			os33<<endl;
		//		}
		//		os33.close();
		//	}
		//}
	}

	delete[] SD;
	SD=NULL;

	//delete[]myOrigin;
	//myOrigin=NULL;

	cvReleaseImage(&CXRGB);
	cvReleaseImage(&CX1);
	cvReleaseImage(&myMap);
	//return myMap;
}
