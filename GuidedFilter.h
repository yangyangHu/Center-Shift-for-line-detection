#include "Global.h"

class GuidedFilter
{
private:
	IplImage* I;//导向图像（the gray-scale haze image）
	IplImage* p;//输入图像（the raw transmission map）
	int r;//局部窗口半径local window radius
	double eps;//正则化系数(regularization parameter)
	int N;//局部窗口内像素的个数
	IplImage* mean_I;
	IplImage* mean_p;
	IplImage* mean_Ip;
	IplImage* cov_Ip;
	IplImage* mean_II;
	IplImage* var_I;
	IplImage* a;
	IplImage* b;
	IplImage* mean_a;
	IplImage* mean_b;
	IplImage* q;//输出图像

public:
	GuidedFilter(IplImage* I1,IplImage* p1,int r1,double eps1);//构造函数

	~GuidedFilter();//析构函数

	void Calculate_q();//计算输出图像q

	IplImage* Getq();//获取q

	void Normalize(IplImage* src,IplImage* dst);//归一化
};