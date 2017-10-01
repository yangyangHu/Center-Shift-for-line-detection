#include "LineDetector.h"

LineDetector::LineDetector(IplImage* in)
{
	//构造函数
	InputImage = cvCreateImage(cvGetSize(in),in->depth,1);
	if (in->nChannels>1)
	{
		cvCvtColor(in,InputImage,CV_BGR2GRAY);
	}
	else
	{
		InputImage = cvCloneImage(in);
	}

	r = 5;

	circularMask_w = cvCreateImage(cvSize(2*r+1,2*r+1),IPL_DEPTH_32F,1);
	circularMask_shift = cvCreateImage(cvGetSize(in),IPL_DEPTH_32F,1);
	OutputImage = cvCreateImage(cvGetSize(in),IPL_DEPTH_8U,1);
}

LineDetector::~LineDetector()//析构函数
{
	cvReleaseImage(&InputImage);
	cvReleaseImage(&circularMask_w);
	cvReleaseImage(&circularMask_shift);
	cvReleaseImage(&OutputImage);
}

void LineDetector::Calculate_w()//计算圆形掩模权值
{
	for (int y=0;y<circularMask_w->height;y++)
	{
		for (int x=0;x<circularMask_w->width;x++)
		{
			int dis;
			dis = (x-r)*(x-r)+(y-r)*(y-r);
			if (dis<=r*r)
			{
				cvSetReal2D(circularMask_w,y,x,1);
			}
			else
			{
				cvSetReal2D(circularMask_w,y,x,0);
			}
		}
	}
	cvShowImage("w",circularMask_w);
}

void LineDetector::Calculate_Gw()//计算圆形掩模权值
{
	for (int y=0;y<circularMask_w->height;y++)
	{
		for (int x=0;x<circularMask_w->width;x++)
		{
			int dis;
			dis = (x-r)*(x-r)+(y-r)*(y-r);
			double power = -((double)dis)/((double)(2*r*r));
			double w = exp(power);
			cvSetReal2D(circularMask_w,y,x,w);
		}
	}
	//归一化
//	cvNormalize(circularMask_w,circularMask_w,1.0,0.0,CV_L1,NULL);
}

void LineDetector::Calculate_Iw()//计算圆形掩模权值(相同的r下，可检测出更宽的暗线,提高检测速度)
{
	for (int y=0;y<circularMask_w->height;y++)
	{
		for (int x=0;x<circularMask_w->width;x++)
		{
			int dis;
			dis = (x-r)*(x-r)+(y-r)*(y-r);
			double power = -((double)dis)/((double)(2*r*r));
			double w = 1 - exp(power);
			cvSetReal2D(circularMask_w,y,x,w);
		}
	}
	//归一化
	//cvNormalize(circularMask_w,circularMask_w,1.0,0.0,CV_L1,NULL);
}

void LineDetector::Calculate_circularMask_shift(int x0,int y0)//计算圆形掩模的重心偏移量
{
	//计算圆形掩摸的重心
	double M00 = 0;//零阶矩
	double M10 = 0;//x一阶矩
	double M01 = 0;//y一阶矩

	double xc;//重心
	double yc;

	double shift;

	for (int y=y0-r;y-y0+r<2*r+1;y++)
	{
		for (int x=x0-r;x-x0+r<2*r+1;x++)
		{
			//图像边界处理
			double pixel;
			int xx = x,yy = y;
			if (xx<0)
			{
				xx = 0;
			}
			if (xx>=InputImage->width)
			{
				xx = InputImage->width - 1;
			}
			if (yy<0)
			{
				yy = 0;
			}
			if (yy>=InputImage->height)
			{
				yy = InputImage->height - 1;
			}
			pixel = cvGetReal2D(InputImage,yy,xx);

			M00 += pixel*cvGetReal2D(circularMask_w,yy-y0+r,xx-x0+r);
			M10 += xx*pixel*cvGetReal2D(circularMask_w,yy-y0+r,xx-x0+r);
			M01 += yy*pixel*cvGetReal2D(circularMask_w,yy-y0+r,xx-x0+r);
		}
	}

	xc = M10/M00;
	yc = M01/M00;

	double dis = (xc-((double)x0))*(xc-((double)x0)) + (yc-((double)y0))*(yc-((double)y0));
	shift = sqrt(dis)/*/((double)r)*/;

	cvSetReal2D(circularMask_shift,y0,x0,shift);
}

void LineDetector::Calculate_shift()//计算重心偏移量
{
	Calculate_w(); //计算圆形掩模权值circularMask_w
	//求WMSB mass
	//for (int y0 = r;y0<InputImage->height-r;y0++)//不考虑边界
	parallel_for (int(r), InputImage->height-r, [&](int y0)
	{
		/*for (int x0 = r;x0<InputImage->width-r;x0++)*/
		parallel_for (int(r), InputImage->width-r, [&](int x0)
		{

			//计算圆形掩模 相似度
			Calculate_circularMask_shift(x0,y0);

		});
	});
	cvShowImage("shift",circularMask_shift);
	cvScale(circularMask_shift,OutputImage,255);
	cvShowImage("out",OutputImage);

	cvLaplace(OutputImage,OutputImage);
	cvShowImage("outLap",OutputImage);

	cvThreshold(OutputImage,OutputImage,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
	cvShowImage("Out2",OutputImage);
}