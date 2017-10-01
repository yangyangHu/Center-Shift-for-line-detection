#include "Global.h"

class GuidedFilter
{
private:
	IplImage* I;//����ͼ��the gray-scale haze image��
	IplImage* p;//����ͼ��the raw transmission map��
	int r;//�ֲ����ڰ뾶local window radius
	double eps;//����ϵ��(regularization parameter)
	int N;//�ֲ����������صĸ���
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
	IplImage* q;//���ͼ��

public:
	GuidedFilter(IplImage* I1,IplImage* p1,int r1,double eps1);//���캯��

	~GuidedFilter();//��������

	void Calculate_q();//�������ͼ��q

	IplImage* Getq();//��ȡq

	void Normalize(IplImage* src,IplImage* dst);//��һ��
};