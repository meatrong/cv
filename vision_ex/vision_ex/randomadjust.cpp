#include <stdlib.h>

#include "head.h"


/* 对图片做随机变换 
 * 歪七扭八各种角度 
*/
Mat transform(Mat img)
{
        int img_height = img.rows;  
        int img_width = img.cols;  
        cout << img_height <<endl;  
        cout << img_width <<endl;  

        vector<Point2f> corners(4);  //原始图像的四个顶点
        corners[0] = Point2f(0,0);  
        corners[1] = Point2f(img_width-1,0);  
        corners[2] = Point2f(0,img_height -1);  
        corners[3] = Point2f(img_width-1,img_height-1); 

		srand(time(0)); 
		int a1 = rand() % 50;
		int b1 = rand() % 50;
		
		int a2 = img_width + rand() % (a1+200);
		int b2 = b1 + rand() % 50;
		int a3 = a1 + rand() % 50;
		int b3 = img_height + rand() % (b1+200);
		int a4 = img_width + rand() % (a1+200);
		int b4 = img_height + rand() % (b1+200);
		cout << a1 << "," << b1 << "\t" << a2 << "," << b2 <<endl;
		cout << a3 << "," << b3 << "\t" << a4 << "," << b4 << endl;

		vector<Point2f> corners_trans(4);  //变换后图像的四个顶点坐标
		corners_trans[0] = Point2f(a1, b1);  
        corners_trans[1] = Point2f(a2, b2);  
        corners_trans[2] = Point2f(a3, b3);  
        corners_trans[3] = Point2f(a4, b4);
    
		int dst_heigh = max(a2,a4);
		int dst_width = max(b3,b4);
		cout << dst_heigh << "," << dst_width << endl;
        Mat transform = getPerspectiveTransform(corners,corners_trans);  
        cout<< transform <<endl;  
        Mat resultImage;
        warpPerspective(img,resultImage,transform,Size(dst_heigh+1 , dst_width+10),INTER_LINEAR);
        //imshow("src",img);
        imshow("randomTrans",resultImage);  
		//imwrite("dst.jpg",resultImage);

		imwrite("./data/test/random.jpg", resultImage);
	
	return resultImage;  
}  


int persadjust(Mat image01, Mat image02){

    //获取图像1到图像2的投影映射矩阵 尺寸为3*3  
    //Mat homo=findHomography(imagePoints2,imagePoints1,CV_RANSAC);  
    Mat homo = bfmatch(image02,image01);
    //cout<<"变换矩阵为：\n"<<homo<<endl<<endl; //输出映射矩阵      
    double adjustValue=image01.cols;  
    Mat adjustMat=(Mat_<double>(3,3)<<1.0,0,35,0,1.0,65,0,0,1.0);  
 
    //图像配准  
    Mat imageTransform1,imageTransform2;  
    warpPerspective(image02,imageTransform1,homo,Size(image01.cols,image01.rows));  

    imshow("透视变换",imageTransform1);
	imwrite("./data/result/adjust.jpg",imageTransform1);

    //waitKey();    
    return 0;    
}
