#include "head.h"

void bug_1();
void bug_2();
void bug_3();
void bug_4();
void bug_5();
//int main()
int debug()
{
	cout << "1.transform; 2.match; 3.adjust; 4.selftest; 5.Calculating similarity;" << endl;
	int bugno;
	cin >> bugno;
	if(bugno == 1){
		bug_1();
	}
	else if(bugno == 2){
		bug_2();
	}
	else if(bugno == 3){
		bug_3();
	}
	else if(bugno == 4){
		bug_4();
	}
	else if(bugno == 5){
		bug_5();
	}
	
	/*Mat image00,image02;
	
	image00 = imread("./testlist/2.jpg");
	Mat image01(DST_IMG_WIDTH,SRC_IMG_HEIGH,image00.type());
	resize(image00,image01 ,image01.size(),0,0,INTER_LINEAR);
	image02 = imread("./rlist/3.jpg");*/

	///////////////////////////transform 变形///////////////////////////////
	/*
		Mat img = imread("./test/1.jpg");  
        int img_height = img.rows;  //行像素数
        int img_width = img.cols;	//列像素数
        cout << img_width << "," << img_height <<endl;  

        vector<Point2f> corners(4);  //原始图像的四个顶点
		corners[0] = Point2f(0,0);  
        corners[1] = Point2f(img_width-1,0);  
        corners[2] = Point2f(0,img_height -1);  
        corners[3] = Point2f(img_width-1,img_height-1);

		srand(time(0)); 
		int a1 = rand() % 50;
		int b1 = rand() % 50;
		
		int a2 = img_width + rand() % (a1+100);
		int b2 = b1 + rand() % 50;
		int a3 = a1 + rand() % 50;
		int b3 = img_height + rand() % (b1+100);
		int a4 = img_width + rand() % (a1+100);
		int b4 = img_height + rand() % (b1+100);
		cout << a1 << "," << b1 << "\t" << a2 << "," << b2 <<endl;
		cout << a3 << "," << b3 << "\t" << a4 << "," << b4 << endl;

		vector<Point2f> corners_trans(4);  //变换后图像的四个顶点坐标
		corners_trans[0] = Point2f(a1, b1);  
        corners_trans[1] = Point2f(a2, b2);  
        corners_trans[2] = Point2f(a3, b3);  
        corners_trans[3] = Point2f(a4, b4);
        
		int dst_height = max(a2,a4);
		int dst_width = max(b3,b4);
		cout << dst_height << "," << dst_width << endl;

        Mat transform = getPerspectiveTransform(corners,corners_trans);  
        cout<< transform <<endl;  
        Mat resultImage;
        warpPerspective(img,resultImage,transform,Size(dst_height + a1 + 1 , dst_width + b1 + 1),INTER_LINEAR);

		dst_height = resultImage.rows;  
        dst_width = resultImage.cols;  
        cout << dst_height <<endl;  
        cout << dst_width <<endl; 

        imshow("src",img);
        imshow("trans",resultImage);  
		imwrite("dst.jpg",resultImage);
	*/
	////////////////////////////////////////////////////////////////////////////////


	////////////////////// surf检查匹配点 //////////////////
	/*
	Mat image1,image2;
	
	//对比度拉伸
	//image1 = equalizetest(image01);
	//image2 = equalizetest(image02);
	
	//灰度图转换 
	cvtColor(image01, image1, CV_RGB2GRAY);
    cvtColor(image02, image2, CV_RGB2GRAY);
    //提取特征点    
    SurfFeatureDetector surfDetector(2000);  // 海塞矩阵阈值，在这里调整精度，值越大点越少，越精准 
    vector<KeyPoint> keyPoint1, keyPoint2;
    surfDetector.detect(image1, keyPoint1);
    surfDetector.detect(image2, keyPoint2);

    //特征点描述，为下边的特征点匹配做准备    
    SurfDescriptorExtractor SurfDescriptor;
    Mat imageDesc1, imageDesc2;
    SurfDescriptor.compute(image1, keyPoint1, imageDesc1);
    SurfDescriptor.compute(image2, keyPoint2, imageDesc2);

    FlannBasedMatcher matcher;
    vector<vector<DMatch> > matchePoints;
    vector<DMatch> GoodMatchePoints;

    vector<Mat> train_desc(1, imageDesc1);
    matcher.add(train_desc);
    matcher.train();

    matcher.knnMatch(imageDesc2, matchePoints, 2);
    cout << "total match points: " << matchePoints.size() << endl;

    // Lowe's algorithm,获取优秀匹配点
    for (int i = 0; i < matchePoints.size(); i++)
    {
        if (matchePoints[i][0].distance < 0.6 * matchePoints[i][1].distance)
        {
            GoodMatchePoints.push_back(matchePoints[i][0]);
        }
    }

    Mat first_match;
    drawMatches(image02, keyPoint2, image01, keyPoint1, GoodMatchePoints, first_match);
    imshow("first_match_surf ", first_match);
	*/
	////////////////////////////////////////////////////////////////////////////////
	///////////////////////////surf 校正///////////////////////////////
	/*
	imshow("原始测试图像",image01);  
    imshow("基准图像",image02);  
  
    //灰度图转换  
    Mat image1,image2;    
    cvtColor(image01,image1,CV_RGB2GRAY);  
    cvtColor(image02,image2,CV_RGB2GRAY);  
  
  
    //提取特征点    
    SurfFeatureDetector surfDetector(800);  // 海塞矩阵阈值  
    vector<KeyPoint> keyPoint1,keyPoint2;    
    surfDetector.detect(image1,keyPoint1);    
    surfDetector.detect(image2,keyPoint2);    
  
    //特征点描述，为下边的特征点匹配做准备    
    SurfDescriptorExtractor SurfDescriptor;    
    Mat imageDesc1,imageDesc2;    
    SurfDescriptor.compute(image1,keyPoint1,imageDesc1);    
    SurfDescriptor.compute(image2,keyPoint2,imageDesc2);      
  
    //获得匹配特征点，并提取最优配对     
    FlannBasedMatcher matcher;  
    vector<DMatch> matchePoints;    
    matcher.match(imageDesc1,imageDesc2,matchePoints,Mat());  
     
	cout << "total match points: " << matchePoints.size() << endl;

	sort(matchePoints.begin(),matchePoints.end()); //特征点排序    
  
    //获取排在前N个的最优匹配特征点  
    vector<Point2f> imagePoints1,imagePoints2;  
  
    for(int i=0;i<10;i++)  
    {         
        imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);       
        imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);       
    } 

    //获取图像1到图像2的投影映射矩阵 尺寸为3*3  
    Mat homo=findHomography(imagePoints1,imagePoints2,CV_RANSAC);  
    ////也可以使用getPerspectiveTransform方法获得透视变换矩阵，不过要求只能有4个点，效果稍差  
    //Mat   homo=getPerspectiveTransform(imagePoints1,imagePoints2);  
    cout<<"变换矩阵为：\n"<<homo<<endl<<endl; //输出映射矩阵      
    double adjustValue=image1.cols;  
    Mat adjustMat=(Mat_<double>(3,3)<<1.0,0,35,0,1.0,65,0,0,1.0);  
    cout<<"调整矩阵为：\n"<<adjustMat<<endl<<endl;  
    cout<<"调整后变换矩阵为：\n"<<adjustMat*homo<<endl;  
  
    //图像配准  
    Mat imageTransform1,imageTransform2;  
    warpPerspective(image01,imageTransform1,homo,Size(image02.cols,image02.rows));  
    warpPerspective(image01,imageTransform2,adjustMat*homo,Size(image02.cols*1.3,image02.rows*1.8));  
    imshow("直接经过透视矩阵变换",imageTransform1);  
    imshow("经过平移修正后的透视矩阵变换",imageTransform2);  
    */
	////////////////////////////////////////////////////////////////////////////////

    waitKey();
    return 0;
}

void bug_1()
{
	
}

/* 试试暴力匹配Brute-force matcher
 * Brute-force matcher就是用暴力方法找到点集一中每个descriptor在点集二中距离最近的descriptor；
 */
void bug_2(){
	
	int test_no;
	char imgname[50];
	cout << "Input error test image number:(1-10)" << endl;
	cin >> test_no;
	while (test_no > 10 || test_no <1 ){
		cout << "Input Error! Please input number between 1-10:" << endl;
		cin >> test_no;
	}
	cout << "You input number is " << test_no << endl;
	sprintf(imgname, "./data/test/sushi%d.jpg", test_no);
	Mat image00 = imread(imgname, 1);  
	Mat image_std;
	
	//image00 = imread("./test/3.jpg");
	//image00 = imread("dst.jpg");
	Mat image01(DST_IMG_WIDTH,SRC_IMG_HEIGH,image00.type());
	resize(image00,image01 ,image01.size(),0,0,INTER_LINEAR);
	image_std = imread("./data/sushi0.jpg");
	image_std = resizeone(image_std, 550);

	imshow("测试图像",image01);  
    imshow("基准图像",image_std);  
  
    //灰度图转换  
    Mat image1,image2;    
    cvtColor(image01,image1,CV_RGB2GRAY);  
    cvtColor(image_std,image2,CV_RGB2GRAY);  
  
  
    //提取特征点    
    SiftFeatureDetector siftDetector;  
    vector<KeyPoint> keyPoint1,keyPoint2;    
    siftDetector.detect(image1,keyPoint1);    
    siftDetector.detect(image2,keyPoint2);    
  
    //特征点描述，为下边的特征点匹配做准备    
    SiftDescriptorExtractor SiftDescriptor;    
    Mat imageDesc1,imageDesc2;    
    SiftDescriptor.compute(image1,keyPoint1,imageDesc1);    
    SiftDescriptor.compute(image2,keyPoint2,imageDesc2);      
  
    //暴力匹配BFMatche 获得匹配特征点，并提取最优配对     
    BFMatcher matcher(NORM_L1, true);  
    vector<DMatch> matchePoints;    
    matcher.match(imageDesc1,imageDesc2,matchePoints,Mat());  
     
	cout << "total match points: " << matchePoints.size() << endl;
	Mat match1;
	drawMatches(image2, keyPoint2, image1, keyPoint1,matchePoints, match1);
	imshow("bf_L1_CROSS", match1);
	waitKey();

	sort(matchePoints.begin(),matchePoints.end()); //特征点排序    
  
    //获取排在前N个的最优匹配特征点  
    vector<Point2f> imagePoints1,imagePoints2;  
  
    for(int i=0;i<10;i++)  
    {         
        imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);       
        imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);       
    } 

	vector<DMatch> GoodMatchePoints;

	
    //获取图像1到图像2的投影映射矩阵 尺寸为3*3  
    Mat homo=findHomography(imagePoints1,imagePoints2,CV_RANSAC);  
    ////也可以使用getPerspectiveTransform方法获得透视变换矩阵，不过要求只能有4个点，效果稍差  
    //Mat   homo=getPerspectiveTransform(imagePoints1,imagePoints2);  
    cout<<"变换矩阵为：\n"<<homo<<endl<<endl; //输出映射矩阵      
    double adjustValue=image1.cols;  
    Mat adjustMat=(Mat_<double>(3,3)<<1.0,0,35,0,1.0,65,0,0,1.0);  
    cout<<"调整矩阵为：\n"<<adjustMat<<endl<<endl;  
    cout<<"调整后变换矩阵为：\n"<<adjustMat*homo<<endl;  
  
    //图像配准  
    Mat imageTransform1,imageTransform2;  
    warpPerspective(image01,imageTransform1,homo,Size(image_std.cols,image_std.rows));  
    warpPerspective(image01,imageTransform2,adjustMat*homo,Size(image_std.cols*1.3,image_std.rows*1.8));  
    imshow("直接经过透视矩阵变换",imageTransform1);  
    imshow("经过平移修正后的透视矩阵变换",imageTransform2);  

}

/* 使用sift+flannmatcher+ warpPerspective透视变换
 * Flann-based matcher 使用快速近似最近邻搜索算法寻找
 * （用快速的第三方库近似最近邻搜索算法）
 * 为了提高检测速度，你可以调用matching函数前，先训练一个matcher。
 * 训练过程可以首先使用cv::FlannBasedMatcher来优化，为descriptor建立索引树，
 * 这种操作将在匹配大量数据时发挥巨大作用（比如在上百幅图像的数据集中查找匹配图像）。
 * 而Brute-force matcher在这个过程并不进行操作，它只是将train descriptors保存在内存中。
 */
void bug_3(){
	Mat image00;
	Mat image_std;
	/*
	cout << "Input error test image number:(1-10)" << endl;
	cin >> test_no;
	while (test_no > 10 || test_no <1 ){
		cout << "Input Error! Please input number between 1-10:" << endl;
		cin >> test_no;
	}
	cout << "You input number is " << test_no << endl;
	sprintf(imgname, "./data/test/sushi%d.jpg", test_no);
	Mat image00 = imread(imgname, 1);  
	
	*/
	//image00 = imread("./test/3.jpg");
	image00 = imread("./data/test/7.jpg");
	
	Mat image01(DST_IMG_WIDTH,SRC_IMG_HEIGH,image00.type());
	resize(image00,image01 ,image01.size(),0,0,INTER_LINEAR);
	image_std = imread("./data/test/match2.jpg");

	imshow("测试图像",image01);  
    imshow("基准图像",image_std);  
  
    //灰度图转换  
    Mat image1,image2;    
    cvtColor(image01,image1,CV_RGB2GRAY);  
    cvtColor(image_std,image2,CV_RGB2GRAY);  
  
  
    //提取特征点    
    SiftFeatureDetector siftDetector;  
    vector<KeyPoint> keyPoint1,keyPoint2;    
    siftDetector.detect(image1,keyPoint1);    
    siftDetector.detect(image2,keyPoint2);    
  
    //特征点描述，为下边的特征点匹配做准备    
    SiftDescriptorExtractor SiftDescriptor;    
    Mat imageDesc1,imageDesc2;    
    SiftDescriptor.compute(image1,keyPoint1,imageDesc1);    
    SiftDescriptor.compute(image2,keyPoint2,imageDesc2);      
  
    //获得匹配特征点，并提取最优配对     
    FlannBasedMatcher matcher;  
    vector<DMatch> matchePoints;    
    matcher.match(imageDesc1,imageDesc2,matchePoints,Mat());  
     
	cout << "total match points: " << matchePoints.size() << endl;

	sort(matchePoints.begin(),matchePoints.end()); //特征点排序    
  
    //获取排在前N个的最优匹配特征点  
    vector<Point2f> imagePoints1,imagePoints2;  
  
    for(int i=0;i<10;i++)  
    {         
        imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);       
        imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);       
    } 

	vector<DMatch> GoodMatchePoints;

    //获取图像1到图像2的投影映射矩阵 尺寸为3*3  
    Mat homo=findHomography(imagePoints1,imagePoints2,CV_RANSAC);  
    ////也可以使用getPerspectiveTransform方法获得透视变换矩阵，不过要求只能有4个点，效果稍差  
    //Mat   homo=getPerspectiveTransform(imagePoints1,imagePoints2);  
    cout<<"变换矩阵为：\n"<<homo<<endl<<endl; //输出映射矩阵      
    double adjustValue=image1.cols;  
    Mat adjustMat=(Mat_<double>(3,3)<<1.0,0,35,0,1.0,65,0,0,1.0);  
    cout<<"调整矩阵为：\n"<<adjustMat<<endl<<endl;  
    cout<<"调整后变换矩阵为：\n"<<adjustMat*homo<<endl;  
  
    //图像配准  
    Mat imageTransform1,imageTransform2;  
    warpPerspective(image01,imageTransform1,homo,Size(image_std.cols,image_std.rows));  
    warpPerspective(image01,imageTransform2,adjustMat*homo,Size(image_std.cols*1.3,image_std.rows*1.8));  
    imshow("直接经过透视矩阵变换",imageTransform1);  
    imshow("经过平移修正后的透视矩阵变换",imageTransform2);  
}
//selftest
void bug_4()
{
	/*
	int i,imgnum;
	char filename[100];
	int img;
	for(i = 1;i <= testnum; i++){
		sprintf(filename, "./testlist/%d.jpg", i);
		Mat image00 = imread(filename);  
	
		Mat image001 = resizeone(image00,DST_IMG_WIDTH);
		//Mat image001(DST_IMG_WIDTH, DST_IMG_WIDTH,image00.type());
		//resize(image00,image001 ,image001.size(),0,0,INTER_LINEAR);

		cout << i << ":" << endl; 
		img = match(image001);
		
		//与正确匹配不对则输出error
		switch(i){
			case 1:if(img != 5) cout << "！！！！！！！！！！！1 error!" << endl;break;
			case 2:if(img != 3) cout << "！！！！！！！！！！！2 error!" << endl;	break;
			case 3:if(img != 12) cout << "！！！！！！！！！！！3 error!" << endl;break;
			case 4:if(img != 15) cout << "！！！！！！！！！！！4 error!" << endl;break;
			case 5:if(img != 14) cout << "！！！！！！！！！！！5 error!" << endl;break;
			case 6:if(img != 6) cout << "！！！！！！！！！！！6 error!" << endl;break;
			case 7:if(img != 13) cout << "！！！！！！！！！！！7 error!" << endl;break;
			case 8:if(img != 4) cout << "！！！！！！！！！！！8 error!" << endl;break;
			case 9:if(img != 2) cout << "！！！！！！！！！！！9 error!" << endl;break;
			case 10:if(img != 1) cout << "！！！！！！！！！！！10 error!" << endl;break;
			case 11:if(img != 7) cout << "！！！！！！！！！！！11 error!" << endl;break;
			case 12:if(img != 10) cout << "！！！！！！！！！！！12 error!" << endl;break;
			case 13:if(img != 8) cout << "！！！！！！！！！！！13 error!" << endl;break;
			
		}
	}
*/
}

//计算图像相似度

void bug_5()
{
/*
	Mat image01, image02 ;
	char filename1[100],filename2[100];
	Mat image1, image2;
	for(int j = 1; j <= testnum ;j++){
		sprintf( filename1,"./testlist/%d.jpg" , j );
		//image01 = imread( "./testlist/2.jpg" );
		image01 = imread( filename1 );
		cout << "image" << j << ":" << endl;
		int lowdiff = 100,ldiffimg = 0;
		for( int i = 1 ; i <= num ;i++ ){
			cout << i << ":" << endl;
			sprintf( filename2, "./rlist/%d.jpg", i);
			image02 = imread( filename2 );
		
			resize(image01, image1, Size(8, 8), 0, 0, INTER_CUBIC);
			resize(image02, image2, Size(8, 8), 0, 0, INTER_CUBIC);
			cvtColor( image1, image1, CV_BGR2GRAY );
			cvtColor( image2, image2, CV_BGR2GRAY );
	
			int iAvg1 = 0, iAvg2 = 0;
			int arr1[64], arr2[64];
 
			for (int i = 0; i < 8; i++) {
				uchar* data1 = image1.ptr<uchar>(i);
				uchar* data2 = image2.ptr<uchar>(i);
 
				int tmp = i * 8;
 
				for (int j = 0; j < 8; j++) {
					int tmp1 = tmp + j;
 
					arr1[tmp1] = data1[j] / 4 * 4;
					arr2[tmp1] = data2[j] / 4 * 4;
 
					iAvg1 += arr1[tmp1];
					iAvg2 += arr2[tmp1];
				}
			}
 
			iAvg1 /= 64;
			iAvg2 /= 64;
 
			for (int i = 0; i < 64; i++) {
				arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
				arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
			}
 
			int iDiffNum = 0;
 
			for (int i = 0; i < 64; i++)
				if (arr1[i] != arr2[i])
					++iDiffNum;
			if(iDiffNum < lowdiff){
				lowdiff = iDiffNum;
				ldiffimg = i;
			}
			cout<<"iDiffNum = "<<iDiffNum<<endl;

		}
		cout<<"ldiffimg = "<< ldiffimg <<endl;
	}*/
}
	/*
		image02 = imread( "./rlist/4.jpg" );
	//image02 = image01.clone();
	Mat image1, image2;
	resize(image01, image1, Size(8, 8), 0, 0, INTER_CUBIC);
	resize(image02, image2, Size(8, 8), 0, 0, INTER_CUBIC);
	
	//imshow( "01", image1 );
	//imshow( "02", image2 );
	//cout << image1.rows << "," << image1.cols << endl;
	//cout << image2.rows << "," << image2.cols << endl;

	
	cvtColor( image1, image1, CV_BGR2GRAY );
	cvtColor( image2, image2, CV_BGR2GRAY );
	
	int iAvg1 = 0, iAvg2 = 0;
	int arr1[64], arr2[64];
 
	for (int i = 0; i < 8; i++) {
		uchar* data1 = image1.ptr<uchar>(i);
		uchar* data2 = image2.ptr<uchar>(i);
 
		int tmp = i * 8;
 
		for (int j = 0; j < 8; j++) {
			int tmp1 = tmp + j;
 
			arr1[tmp1] = data1[j] / 4 * 4;
			arr2[tmp1] = data2[j] / 4 * 4;
 
			iAvg1 += arr1[tmp1];
			iAvg2 += arr2[tmp1];
		}
	}
 
	iAvg1 /= 64;
	iAvg2 /= 64;
 
	for (int i = 0; i < 64; i++) {
		arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
		arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
	}
 
	int iDiffNum = 0;
 
	for (int i = 0; i < 64; i++)
		if (arr1[i] != arr2[i])
			++iDiffNum;
 
	cout<<"iDiffNum = "<<iDiffNum<<endl;
 
	if (iDiffNum <= 5)
		cout<<"two images are very similar!"<<endl;
	else if (iDiffNum > 10)
		cout<<"they are two different images!"<<endl;
	else
		cout<<"two image are somewhat similar!"<<endl;
	*/

	/////////////////////////  perceptual hash algorithm ////////////////////////
	//string strSrcImageName = "dst.jpg";
 /*
	Mat matSrc, matSrc1, matSrc2;
 
	matSrc = imread(strSrcImageName, CV_LOAD_IMAGE_COLOR);
	CV_Assert(matSrc.channels() == 3);
 
	resize(matSrc, matSrc1, Size(357, 419), 0, 0, INTER_NEAREST);
	//cv::flip(matSrc1, matSrc1, 1);
	resize(matSrc, matSrc2, Size(2177, 3233), 0, 0, INTER_LANCZOS4);
 
	Mat matDst1, matDst2;
 
	resize(matSrc1, matDst1, Size(8, 8), 0, 0, INTER_CUBIC);
	resize(matSrc2, matDst2, Size(8, 8), 0, 0, INTER_CUBIC);
 
	cvtColor(matDst1, matDst1, CV_BGR2GRAY);
	cvtColor(matDst2, matDst2, CV_BGR2GRAY);

 */
void bug_6(){
/*if (enableRatioTest)
	{
		// To avoid NaNs when best match has 
		// zero distance we will use inverse ratio. 
		const float minRatio = 1.f / 1.5f;
		// KNN match will return 2 nearest 
		// matches for each query descriptor
		m_matcher->knnMatch(queryDescriptors, m_knnMatches, 2);
		for (size_t i=0; i<m_knnMatches.size(); i++)
		{
			const cv::DMatch& bestMatch = m_knnMatches[i][0];
			const cv::DMatch& betterMatch = m_knnMatches[i][1];
			float distanceRatio = bestMatch.distance / 
				betterMatch.distance;
			// Pass only matches where distance ratio between 
			// nearest matches is greater than 1.5 
			// (distinct criteria)
			if (distanceRatio < minRatio)
			{
				matches.push_back(bestMatch);
			}
		}
	}
	else
	{
		// Perform regular match
		m_matcher->match(queryDescriptors, matches);
	}
	*/
}