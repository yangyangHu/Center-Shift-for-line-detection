#include "Global.h"

class LineDetector 
{
	//成员变量
private:
	IplImage* InputImage;//输入的舌灰度图像
	IplImage* circularMask_w;//圆形掩模 权值
	IplImage* circularMask_shift;//圆形掩模的重心偏移量
	IplImage* OutputImage;//输出
	int r;//圆形掩模的半径

	//成员函数
public:
	LineDetector(IplImage* in);//构造函数
	~LineDetector();//析构函数
	void Calculate_w();//计算圆形掩模权值

	void Calculate_Gw();//计算圆形掩模权值
	void Calculate_Iw();//计算圆形掩模权值(相同的r下，可检测出更宽的暗线,提高检测速度)

	void Calculate_circularMask_shift(int x0,int y0);//计算圆形掩模的重心偏移量
	void Calculate_shift();//计算重心偏移量
};