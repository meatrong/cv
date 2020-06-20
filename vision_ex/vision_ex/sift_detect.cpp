#include "head.h"

/* ������ */
int extract()
//int main()
{
	Mat image1, image2, image01, image02;

		//��ȡ�Ҷ����Ա任���ģ���������
		//sprintf(filename, "./rlist/%d.jpg", i);
		image01 = imread("./data/ucas1.jpg", 1);
		image02 = imread("./data/ucas2.jpg", 1);

		image1 = resizeone(image01, 600);
		image2 = resizeone(image02, 600);
	
		//��ȡ������    
		SiftFeatureDetector siftDetector; 
		
		vector<KeyPoint> keyPoint1, keyPoint2;
		siftDetector.detect(image1, keyPoint1);
		siftDetector.detect(image2, keyPoint2);
		
		//sprintf(descnum, "imageDesc%d", i);
		//������������Ϊ�±ߵ�������ƥ����׼��    
		SiftDescriptorExtractor siftExtractor;
		Mat imageDesc1, imageDesc2;	//descriptor
		siftExtractor.compute(image1, keyPoint1, imageDesc1);
		siftExtractor.compute(image2, keyPoint2, imageDesc2);

		Mat res1,res2; 
		int drawmode = DrawMatchesFlags::DRAW_RICH_KEYPOINTS;
		drawKeypoints(image1, keyPoint1,res1,Scalar::all(-1),drawmode);//���ڴ��л���������
		drawKeypoints(image2, keyPoint2,res2,Scalar::all(-1),drawmode);

		cout<<"size of description of Img1: "<<keyPoint1.size()<<endl;
		cout<<"size of description of Img2: "<<keyPoint2.size()<<endl;
		

	//write the size of features on picture
	CvFont font;    
	double hScale=1;   
	double vScale=1;    
	int lineWidth=2;// �൱��д�ֵ�����    
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);//��ʼ�����壬׼��д��ͼƬ�ϵ�   
	// cvPoint Ϊ��ʵ�x��y����   
	IplImage* transimg1 = cvCloneImage(&(IplImage) res1);
	IplImage* transimg2 = cvCloneImage(&(IplImage) res2);
		
	char str1[20],str2[20];
	sprintf(str1,"%d",keyPoint1.size());
	sprintf(str2,"%d",keyPoint2.size());
 
 
	const char* str = str1;
	cvPutText(transimg1,str1,cvPoint(280,230),&font,CV_RGB(255,0,0));//��ͼƬ������ַ� 
 
	str = str2;
	cvPutText(transimg2,str2,cvPoint(280,230),&font,CV_RGB(255,0,0));//��ͼƬ������ַ� 
 
	//imshow("Description 1",transimg1);
	cvShowImage("descriptor1",transimg1);	//cvShowImage��ʾIplImage*���ͣ�imshow��ʾmat���ͣ�
	cvShowImage("descriptor2",transimg2);
	
	return 0;

}