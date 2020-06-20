#include "head.h"

/* 提特征 */
int extract()
//int main()
{
	Mat image1, image2, image01, image02;

		//提取灰度线性变换后的模板的特征点
		//sprintf(filename, "./rlist/%d.jpg", i);
		image01 = imread("./data/ucas1.jpg", 1);
		image02 = imread("./data/ucas2.jpg", 1);

		image1 = resizeone(image01, 600);
		image2 = resizeone(image02, 600);
	
		//提取特征点    
		SiftFeatureDetector siftDetector; 
		
		vector<KeyPoint> keyPoint1, keyPoint2;
		siftDetector.detect(image1, keyPoint1);
		siftDetector.detect(image2, keyPoint2);
		
		//sprintf(descnum, "imageDesc%d", i);
		//特征点描述，为下边的特征点匹配做准备    
		SiftDescriptorExtractor siftExtractor;
		Mat imageDesc1, imageDesc2;	//descriptor
		siftExtractor.compute(image1, keyPoint1, imageDesc1);
		siftExtractor.compute(image2, keyPoint2, imageDesc2);

		Mat res1,res2; 
		int drawmode = DrawMatchesFlags::DRAW_RICH_KEYPOINTS;
		drawKeypoints(image1, keyPoint1,res1,Scalar::all(-1),drawmode);//在内存中画出特征点
		drawKeypoints(image2, keyPoint2,res2,Scalar::all(-1),drawmode);

		cout<<"size of description of Img1: "<<keyPoint1.size()<<endl;
		cout<<"size of description of Img2: "<<keyPoint2.size()<<endl;
		

	//write the size of features on picture
	CvFont font;    
	double hScale=1;   
	double vScale=1;    
	int lineWidth=2;// 相当于写字的线条    
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);//初始化字体，准备写到图片上的   
	// cvPoint 为起笔的x，y坐标   
	IplImage* transimg1 = cvCloneImage(&(IplImage) res1);
	IplImage* transimg2 = cvCloneImage(&(IplImage) res2);
		
	char str1[20],str2[20];
	sprintf(str1,"%d",keyPoint1.size());
	sprintf(str2,"%d",keyPoint2.size());
 
 
	const char* str = str1;
	cvPutText(transimg1,str1,cvPoint(280,230),&font,CV_RGB(255,0,0));//在图片中输出字符 
 
	str = str2;
	cvPutText(transimg2,str2,cvPoint(280,230),&font,CV_RGB(255,0,0));//在图片中输出字符 
 
	//imshow("Description 1",transimg1);
	cvShowImage("descriptor1",transimg1);	//cvShowImage显示IplImage*类型，imshow显示mat类型；
	cvShowImage("descriptor2",transimg2);
	
	return 0;

}