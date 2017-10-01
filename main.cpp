#include "GuidedFilter.h"
#include "LineDetector.h"

int main()
{
	IplImage* inputImage = cvLoadImage("E:\\上海中医药大学合作项目\\projects\\舌像特征提取\\裂纹\\data\\23.jpg",CV_LOAD_IMAGE_COLOR);
	IplImage* grayImage = cvCreateImage(cvGetSize(inputImage),8,1);
	cvCvtColor(inputImage,grayImage,CV_BGR2GRAY);

	GuidedFilter guidedfilter = GuidedFilter(grayImage,grayImage,5,0.001);
	IplImage* q = guidedfilter.Getq();

	IplImage* qq = cvCreateImage(cvGetSize(q),8,1);
	for (int y=0;y<q->height;y++)
	{
		for (int x=0;x<q->width;x++)
		{
			int value = cvRound(255*cvGetReal2D(q,y,x));
			cvSetReal2D(qq,y,x,value);
		}
	}

	LineDetector linedetector(qq);
	linedetector.Calculate_shift();



	cvShowImage("in",inputImage);
	cvShowImage("gray",grayImage);

	cvShowImage("q",qq);
	cvWaitKey(0);
	return 0;
}
