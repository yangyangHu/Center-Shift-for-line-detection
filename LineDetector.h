#include "Global.h"

class LineDetector 
{
	//��Ա����
private:
	IplImage* InputImage;//�������Ҷ�ͼ��
	IplImage* circularMask_w;//Բ����ģ Ȩֵ
	IplImage* circularMask_shift;//Բ����ģ������ƫ����
	IplImage* OutputImage;//���
	int r;//Բ����ģ�İ뾶

	//��Ա����
public:
	LineDetector(IplImage* in);//���캯��
	~LineDetector();//��������
	void Calculate_w();//����Բ����ģȨֵ

	void Calculate_Gw();//����Բ����ģȨֵ
	void Calculate_Iw();//����Բ����ģȨֵ(��ͬ��r�£��ɼ�������İ���,��߼���ٶ�)

	void Calculate_circularMask_shift(int x0,int y0);//����Բ����ģ������ƫ����
	void Calculate_shift();//��������ƫ����
};