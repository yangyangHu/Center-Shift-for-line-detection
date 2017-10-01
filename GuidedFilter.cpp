
#include "GuidedFilter.h"

//构造函数
GuidedFilter::GuidedFilter(IplImage* I1,IplImage* p1,int r1,double eps1)
{
	//导向图和预估的透射率图都必须从[0,255]先映射到[0,1]在进行计算
	I = cvCreateImage(cvGetSize(I1),IPL_DEPTH_32F,1);
	//对I1做归一化处理
	Normalize(I1,I);
	//cvNormalize(I1,I,1.0,0.0,CV_C);
	//p = cvCloneImage(p1);

	p = cvCreateImage(cvGetSize(p1),IPL_DEPTH_32F,1);
	Normalize(p1,p);

	//cvNormalize(p,p,1.0,0.0,CV_C);
	r = r1;
	eps = eps1;
	N = (2*r+1)*(2*r+1);
	
	mean_I = cvCreateImage(cvGetSize(I1),IPL_DEPTH_32F,1);
	mean_p = cvCreateImage(cvGetSize(I1),IPL_DEPTH_32F,1);
	mean_Ip = cvCreateImage(cvGetSize(I1),IPL_DEPTH_32F,1);
	cov_Ip = cvCreateImage(cvGetSize(I1),IPL_DEPTH_32F,1);
	mean_II = cvCreateImage(cvGetSize(I1),IPL_DEPTH_32F,1);
	var_I = cvCreateImage(cvGetSize(I1),IPL_DEPTH_32F,1);
	a = cvCreateImage(cvGetSize(I1),IPL_DEPTH_32F,1);
	b = cvCreateImage(cvGetSize(I1),IPL_DEPTH_32F,1);
	mean_a = cvCreateImage(cvGetSize(I1),IPL_DEPTH_32F,1);
	mean_b = cvCreateImage(cvGetSize(I1),IPL_DEPTH_32F,1);
	q = cvCreateImage(cvGetSize(I1),IPL_DEPTH_32F,1);//输出图像
}

//析构函数
GuidedFilter::~GuidedFilter()
{
	cvReleaseImage(&I);
	cvReleaseImage(&p);
	cvReleaseImage(&mean_I);
	cvReleaseImage(&mean_p);
	cvReleaseImage(&mean_Ip);
	cvReleaseImage(&cov_Ip);
	cvReleaseImage(&mean_II);
	cvReleaseImage(&var_I);
	cvReleaseImage(&a);
	cvReleaseImage(&b);
	cvReleaseImage(&mean_a);
	cvReleaseImage(&mean_b);
	cvReleaseImage(&q);
}

//计算输出图像q
void GuidedFilter::Calculate_q()
{
	//I均值模糊
	cvSmooth(I,mean_I,CV_BLUR,2*r+1,2*r+1);

	//p均值模糊
	cvSmooth(p,mean_p,CV_BLUR,2*r+1,2*r+1);

	//Ip均值模糊
	cvMul(I,p,mean_Ip);
	cvSmooth(mean_Ip,mean_Ip,CV_BLUR,2*r+1,2*r+1);

	//求cov_Ip
	IplImage* Mul_meanI_mean_p = cvCreateImage(cvGetSize(I),IPL_DEPTH_32F,1);
	cvMul(mean_I,mean_p,Mul_meanI_mean_p);
	cvAbsDiff(mean_Ip,Mul_meanI_mean_p,cov_Ip);
	cvReleaseImage(&Mul_meanI_mean_p);

	//求mean_II
	IplImage* II = cvCreateImage(cvGetSize(I),IPL_DEPTH_32F,1);
	cvMul(I,I,II);
	cvSmooth(II,mean_II,CV_BLUR,2*r+1,2*r+1);
	cvReleaseImage(&II);

	//求var_I
	IplImage* Mul_meanI_mean_I = cvCreateImage(cvGetSize(I),IPL_DEPTH_32F,1);
	cvMul(mean_I,mean_I,Mul_meanI_mean_I);
	cvAbsDiff(mean_II,Mul_meanI_mean_I,var_I);
	cvReleaseImage(&Mul_meanI_mean_I);

	//求a
	cvAddS(var_I,cvScalarAll(eps),var_I);
	cvDiv(cov_Ip,var_I,a);

	//求b
	IplImage* Mul_a_mean_I = cvCreateImage(cvGetSize(I),IPL_DEPTH_32F,1);
	cvMul(a,mean_I,Mul_a_mean_I);
	cvAbsDiff(mean_p,Mul_a_mean_I,b);
	cvReleaseImage(&Mul_a_mean_I);

	//求mean_a
	cvSmooth(a,mean_a,CV_BLUR,2*r+1,2*r+1);

	//求mean_b
	cvSmooth(b,mean_b,CV_BLUR,2*r+1,2*r+1);

	//求q
	IplImage* Mul_mean_a_I = cvCreateImage(cvGetSize(I),IPL_DEPTH_32F,1);
	cvMul(mean_a,I,Mul_mean_a_I);
	cvAdd(Mul_mean_a_I,mean_b,q);
	cvReleaseImage(&Mul_mean_a_I);
}

//获取q
IplImage* GuidedFilter::Getq()
{
	Calculate_q();
	return q;
}

//归一化
void GuidedFilter::Normalize(IplImage* src,IplImage* dst)
{
	for (int y=0;y<src->height;y++)
	{
		for (int x=0;x<src->width;x++)
		{
			double pixel = cvGetReal2D(src,y,x)/255;
			cvSetReal2D(dst,y,x,pixel);
		}
	}
}