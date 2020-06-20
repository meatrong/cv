#include "head.h"


four_corners_t corners;

void CalcCorners(const Mat& H, const Mat& src){
    double v2[] = { 0, 0, 1 };//���Ͻ�
    double v1[3];//�任�������ֵ
    Mat V2 = Mat(3, 1, CV_64FC1, v2);  //������
    Mat V1 = Mat(3, 1, CV_64FC1, v1);  //������

    V1 = H * V2;
    //���Ͻ�(0,0,1)
	//cout << "V2: " << V2 << endl;
    //cout << "V1: " << V1 << endl;
    corners.left_top.x = v1[0] / v1[2];
    corners.left_top.y = v1[1] / v1[2];

    //���½�(0,src.rows,1)
    v2[0] = 0;
    v2[1] = src.rows;
    v2[2] = 1;
    V2 = Mat(3, 1, CV_64FC1, v2);  //������
    V1 = Mat(3, 1, CV_64FC1, v1);  //������
    V1 = H * V2;
    corners.left_bottom.x = v1[0] / v1[2];
    corners.left_bottom.y = v1[1] / v1[2];

    //���Ͻ�(src.cols,0,1)
    v2[0] = src.cols;
    v2[1] = 0;
    v2[2] = 1;
    V2 = Mat(3, 1, CV_64FC1, v2);  //������
    V1 = Mat(3, 1, CV_64FC1, v1);  //������
    V1 = H * V2;
    corners.right_top.x = v1[0] / v1[2];
    corners.right_top.y = v1[1] / v1[2];

    //���½�(src.cols,src.rows,1)
    v2[0] = src.cols;
    v2[1] = src.rows;
    v2[2] = 1;
    V2 = Mat(3, 1, CV_64FC1, v2);  //������
    V1 = Mat(3, 1, CV_64FC1, v1);  //������
    V1 = H * V2;
    corners.right_bottom.x = v1[0] / v1[2];
    corners.right_bottom.y = v1[1] / v1[2];

}

//�Ż���ͼ�����Ӵ���ʹ��ƴ����Ȼ
void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst){
    int start = MIN(corners.left_top.x, corners.left_bottom.x);//��ʼλ�ã����ص��������߽�  

    double processWidth = img1.cols - start;//�ص�����Ŀ��  
    int rows = dst.rows;
    int cols = img1.cols; //ע�⣬������*ͨ����
    double alpha = 1;//img1�����ص�Ȩ��  
    for (int i = 0; i < rows; i++)
    {
        uchar* p = img1.ptr<uchar>(i);  //��ȡ��i�е��׵�ַ
        uchar* t = trans.ptr<uchar>(i);
        uchar* d = dst.ptr<uchar>(i);
        for (int j = start; j < cols; j++)
        {
            //�������ͼ��trans�������صĺڵ㣬����ȫ����img1�е�����
            if (t[j * 3] == 0 && t[j * 3 + 1] == 0 && t[j * 3 + 2] == 0)
            {
                alpha = 1;
            }
            else
            {
                //img1�����ص�Ȩ�أ��뵱ǰ�������ص�������߽�ľ�������ȣ�ʵ��֤�������ַ���ȷʵ��  
                alpha = (processWidth - (j - start)) / processWidth;
            }

            d[j * 3] = p[j * 3] * alpha + t[j * 3] * (1 - alpha);
            d[j * 3 + 1] = p[j * 3 + 1] * alpha + t[j * 3 + 1] * (1 - alpha);
            d[j * 3 + 2] = p[j * 3 + 2] * alpha + t[j * 3 + 2] * (1 - alpha);

        }
    }

}

int stitch(Mat image01, Mat image02){
	//�Ҷ�ͼת��  
    Mat image1,image2;    
    cvtColor(image01,image1,CV_RGB2GRAY);  
	cvtColor(image02,image2,CV_RGB2GRAY);  

	//��ȡ������    
	SiftFeatureDetector siftDetector; 
	vector<KeyPoint> keyPoint1, keyPoint2;
	siftDetector.detect(image1, keyPoint1);
	siftDetector.detect(image2, keyPoint2);
	//������������Ϊ�±ߵ�������ƥ����׼��    
	SiftDescriptorExtractor siftExtractor;
	Mat imageDesc1, imageDesc2;	//descriptor
	siftExtractor.compute(image1, keyPoint1, imageDesc1);
	siftExtractor.compute(image2, keyPoint2, imageDesc2);

    //���ƥ�������㣬����ȡ�������     
    FlannBasedMatcher matcher;  
    vector<DMatch> matchePoints;    
    matcher.match(imageDesc1,imageDesc2,matchePoints,Mat());  
    sort(matchePoints.begin(),matchePoints.end()); //����������    

    //��ȡ����ǰN��������ƥ��������  
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

	//��ȡͼ��1��ͼ��2��ͶӰӳ����� �ߴ�Ϊ3*3  
    Mat homo=findHomography(imagePoints2,imagePoints1,CV_RANSAC);  
    
    cout<<"�任����Ϊ��\n"<<homo<<endl<<endl; //���ӳ�����      
   
	//������׼ͼ���ĸ���������
	CalcCorners(homo, image01);
    cout << "left_top:" << corners.left_top << endl;
    cout << "left_bottom:" << corners.left_bottom << endl;
    cout << "right_top:" << corners.right_top << endl;
    cout << "right_bottom:" << corners.right_bottom << endl;
 
    
	//ƴ��ͼ��׼
	Mat imageStitchTrans2, imageStitchTrans1;
    warpPerspective(image02, imageStitchTrans2, homo, Size(MAX(corners.right_top.x, corners.right_bottom.x), image01.rows));
  
	 //����ƴ�Ӻ��ͼ,����ǰ����ͼ�Ĵ�С
    int dst_width = imageStitchTrans2.cols;  //ȡ���ҵ�ĳ���Ϊƴ��ͼ�ĳ���
    int dst_height = image01.rows;

    Mat dst(dst_height, dst_width, CV_8UC3);
    dst.setTo(0);

    imageStitchTrans2.copyTo(dst(Rect(0, 0, imageStitchTrans2.cols, imageStitchTrans2.rows)));
    image01.copyTo(dst(Rect(0, 0, image01.cols, image01.rows)));

    //imshow("b_dst", dst);
    OptimizeSeam(image01, imageStitchTrans2, dst);

    imshow("ƴ�Ӻ��ͼƬ", dst);
    imwrite("./data/result/stitch.jpg", dst);

	return 0;
}