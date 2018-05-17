#pragma once
#include <iostream> //��׼����/���
#include <stdio.h>

// OpenCV ��
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

struct PointC {
	double x;
	double y;
	double z;
};
//struct Color {
//	int r;
//	int g;
//	int b;
//};
// ����ڲ�
const double camera_factor = 1000;
const double camera_cx = 325.5;
const double camera_cy = 253.5;
const double camera_fx = 518.0;
const double camera_fy = 519.0;

void CalculatePoint(cv::Mat depth, PointC *p);