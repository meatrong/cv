#include "head.h"


four_corners_t corners;

void CalcCorners(const Mat& H, const Mat& src){
    double v2[] = { 0, 0, 1 };//左上角
    double v1[3];//变换后的坐标值
    Mat V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
    Mat V1 = Mat(3, 1, CV_64FC1, v1);  //列向量

    V1 = H * V2;
    //左上角(0,0,1)
	//cout << "V2: " << V2 << endl;
    //cout << "V1: " << V1 << endl;
    corners.left_top.x = v1[0] / v1[2];
    corners.left_top.y = v1[1] / v1[2];

    //左下角(0,src.rows,1)
    v2[0] = 0;
    v2[1] = src.rows;
    v2[2] = 1;
    V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
    V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
    V1 = H * V2;
    corners.left_bottom.x = v1[0] / v1[2];
    corners.left_bottom.y = v1[1] / v1[2];

    //右上角(src.cols,0,1)
    v2[0] = src.cols;
    v2[1] = 0;
    v2[2] = 1;
    V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
    V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
    V1 = H * V2;
    corners.right_top.x = v1[0] / v1[2];
    corners.right_top.y = v1[1] / v1[2];

    //右下角(src.cols,src.rows,1)
    v2[0] = src.cols;
    v2[1] = src.rows;
    v2[2] = 1;
    V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
    V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
    V1 = H * V2;
    corners.right_bottom.x = v1[0] / v1[2];
    corners.right_bottom.y = v1[1] / v1[2];

}

//优化两图的连接处，使得拼接自然
void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst){
    int start = MIN(corners.left_top.x, corners.left_bottom.x);//开始位置，即重叠区域的左边界  

    double processWidth = img1.cols - start;//重叠区域的宽度  
    int rows = dst.rows;
    int cols = img1.cols; //注意，是列数*通道数
    double alpha = 1;//img1中像素的权重  
    for (int i = 0; i < rows; i++)
    {
        uchar* p = img1.ptr<uchar>(i);  //获取第i行的首地址
        uchar* t = trans.ptr<uchar>(i);
        uchar* d = dst.ptr<uchar>(i);
        for (int j = start; j < cols; j++)
        {
            //如果遇到图像trans中无像素的黑点，则完全拷贝img1中的数据
            if (t[j * 3] == 0 && t[j * 3 + 1] == 0 && t[j * 3 + 2] == 0)
            {
                alpha = 1;
            }
            else
            {
                //img1中像素的权重，与当前处理点距重叠区域左边界的距离成正比，实验证明，这种方法确实好  
                alpha = (processWidth - (j - start)) / processWidth;
            }

            d[j * 3] = p[j * 3] * alpha + t[j * 3] * (1 - alpha);
            d[j * 3 + 1] = p[j * 3 + 1] * alpha + t[j * 3 + 1] * (1 - alpha);
            d[j * 3 + 2] = p[j * 3 + 2] * alpha + t[j * 3 + 2] * (1 - alpha);

        }
    }

}

int stitch(Mat image01, Mat image02){
	//灰度图转换  
    Mat image1,image2;    
    cvtColor(image01,image1,CV_RGB2GRAY);  
	cvtColor(image02,image2,CV_RGB2GRAY);  

	//提取特征点    
	SiftFeatureDetector siftDetector; 
	vector<KeyPoint> keyPoint1, keyPoint2;
	siftDetector.detect(image1, keyPoint1);
	siftDetector.detect(image2, keyPoint2);
	//特征点描述，为下边的特征点匹配做准备    
	SiftDescriptorExtractor siftExtractor;
	Mat imageDesc1, imageDesc2;	//descriptor
	siftExtractor.compute(image1, keyPoint1, imageDesc1);
	siftExtractor.compute(image2, keyPoint2, imageDesc2);

    //获得匹配特征点，并提取最优配对     
    FlannBasedMatcher matcher;  
    vector<DMatch> matchePoints;    
    matcher.match(imageDesc1,imageDesc2,matchePoints,Mat());  
    sort(matchePoints.begin(),matchePoints.end()); //特征点排序    

    //获取排在前N个的最优匹配特征点  
    vector<Point2f> imagePoints1,imagePoints2;  
  
    for(int i=0;i<10;i++)  
    {         
        imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);       
        imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);       
    }  
  
	vector<DMatch> goodMatchePoints;  
    for(int i=0;i<10;i++)  
    {         
        goodMatchePoints.push_back(matchePoints[i]);          
    }  

	//获取图像1到图像2的投影映射矩阵 尺寸为3*3  
    Mat homo=findHomography(imagePoints2,imagePoints1,CV_RANSAC);  
    
    cout<<"变换矩阵为：\n"<<homo<<endl<<endl; //输出映射矩阵      
   
	//计算配准图的四个顶点坐标
	CalcCorners(homo, image01);
    cout << "left_top:" << corners.left_top << endl;
    cout << "left_bottom:" << corners.left_bottom << endl;
    cout << "right_top:" << corners.right_top << endl;
    cout << "right_bottom:" << corners.right_bottom << endl;
 
    
	//拼接图配准
	Mat imageStitchTrans2, imageStitchTrans1;
    warpPerspective(image02, imageStitchTrans2, homo, Size(MAX(corners.right_top.x, corners.right_bottom.x), image01.rows));
  
	 //创建拼接后的图,需提前计算图的大小
    int dst_width = imageStitchTrans2.cols;  //取最右点的长度为拼接图的长度
    int dst_height = image01.rows;

    Mat dst(dst_height, dst_width, CV_8UC3);
    dst.setTo(0);

    imageStitchTrans2.copyTo(dst(Rect(0, 0, imageStitchTrans2.cols, imageStitchTrans2.rows)));
    image01.copyTo(dst(Rect(0, 0, image01.cols, image01.rows)));

    //imshow("b_dst", dst);
    OptimizeSeam(image01, imageStitchTrans2, dst);

    imshow("拼接后的图片", dst);
    imwrite("./data/result/stitch.jpg", dst);

	return 0;
}