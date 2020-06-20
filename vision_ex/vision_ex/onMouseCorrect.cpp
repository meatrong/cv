#include "core/core.hpp"
#include "highgui/highgui.hpp"
#include "imgproc/imgproc.hpp"
 
using namespace cv;
 
void onMouse(int event,int x,int y,int flags,void *utsc);
Point2f srcTri[4],dstTri[4];
int clickTimes=0;  //��ͼ���ϵ�������
Mat image;
Mat imageWarp;
int clickcorrect()
{
	image=imread("./data/test/ucas3.jpg");
	imshow("Source Image",image);
	setMouseCallback("Source Image",onMouse);
	waitKey();
	return 0;
}
 
void onMouse(int event,int x,int y,int flags,void *utsc)
{
	if(event==CV_EVENT_LBUTTONUP)   //��Ӧ������̧���¼�
	{
		circle(image,Point(x,y),2.5,Scalar(0,0,255),2.5);  //���ѡ�е�
		imshow("Source Image",image);
		srcTri[clickTimes].x=x;
		srcTri[clickTimes].y=y;
		clickTimes++;
	}
	if(clickTimes==4)
	{
		dstTri[0].x=0;
		dstTri[0].y=0;
		dstTri[1].x=image.rows-1;
		dstTri[1].y=0;
		dstTri[2].x=0;
		dstTri[2].y=image.cols-161;  
		dstTri[3].x=image.rows-1;
		dstTri[3].y=image.cols-161;
		Mat transform=Mat::zeros(3,3,CV_32FC1); //͸�ӱ任����
		transform=getPerspectiveTransform(srcTri,dstTri);  //��ȡ͸�ӱ任����		
		warpPerspective(image,imageWarp,transform,Size(image.rows,image.cols-160));  //͸�ӱ任
		imshow("�ֶ�У�����ͼ��",imageWarp);
		imwrite("./data/result/mousecor.jpg",imageWarp);
	}
}