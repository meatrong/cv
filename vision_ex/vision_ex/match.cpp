#include "head.h"

int matchone(Mat src_image, Mat test_image)
{
	Mat gsrc_image;
	cvtColor(src_image, gsrc_image, CV_RGB2GRAY);	//src转灰度
	
	Mat  gtest_image; 
	cvtColor(test_image, gtest_image, CV_RGB2GRAY);	//src转灰度

	//提取特征点 (使用的是灰度图像)   
	SiftFeatureDetector siftDetector;   
	vector<KeyPoint> src_keyPoint, test_keyPoint;
	siftDetector.detect(gsrc_image, src_keyPoint);
	siftDetector.detect(gtest_image, test_keyPoint);
	
	//特征点描述，为下边的特征点匹配做准备    
    SiftDescriptorExtractor SiftDescriptor;
    Mat src_imageDesc, test_imageDesc;
    SiftDescriptor.compute(gsrc_image, src_keyPoint, src_imageDesc);
	SiftDescriptor.compute(gtest_image, test_keyPoint, test_imageDesc);
		
		FlannBasedMatcher matcher;
		vector<vector<DMatch> > matchePoints;
		vector<DMatch> GoodMatchePoints;

		vector<Mat> train_desc(1, src_imageDesc);
		matcher.add(train_desc);
		matcher.train();

		matcher.knnMatch(test_imageDesc, matchePoints, 2);
		//cout << "total match points : " << matchePoints.size() << endl;

		// Lowe's algorithm,获取优秀匹配点
		for (int j = 0; j < matchePoints.size(); j++)
		{
			if (matchePoints[j][0].distance < 0.6 * matchePoints[j][1].distance)
			{
				GoodMatchePoints.push_back(matchePoints[j][0]);
			}
		}

		//cout << "Good match points :" << GoodMatchePoints.size() << endl;
		
		if(GoodMatchePoints.size() < 10){
			cout << "测试图片与原图无法匹配！" << endl;
			imshow("原始图像", src_image);  
			imshow("测试图像", test_image);
			waitKey(0);
			exit(0);
		}

		 //此处原有绘制最优匹配点  
		Mat match;
		drawMatches(test_image, test_keyPoint, src_image, src_keyPoint, GoodMatchePoints, match);
		imshow("match_image ", match);
		//waitKey(0);

   
   return 0;
 
}

Mat bfmatch(Mat image_src, Mat image_test){

	int hessians = 0;

	Mat image1, image2, image01, image02;

	//image01 = imread("./pic/sushi0.jpg", 1);
	//image02 = imread("./pic/sushi3.jpg", 1);

	image1 = resizeone(image_src, 600);
	image2 = resizeone(image_test, 600);
	
	//提取特征点 
	//1.初始化
	SiftFeatureDetector siftDetector; 	
	vector<KeyPoint> keyPoint1, keyPoint2;
	// 2.提取特征点
	siftDetector.detect(image1, keyPoint1);
	siftDetector.detect(image2, keyPoint2);
	Mat img_kp1, img_kp2;
	drawKeypoints(image1, keyPoint1, img_kp1);
	drawKeypoints(image2, keyPoint2, img_kp2);
	cout<<"size of description of Img1: "<<keyPoint1.size()<<endl;
	cout<<"size of description of Img2: "<<keyPoint2.size()<<endl;
	//3.计算特征点描述，为下边的特征点匹配做准备    
	SiftDescriptorExtractor siftExtractor;
	Mat imageDesc1, imageDesc2;	//descriptor
	siftExtractor.compute(image1, keyPoint1, imageDesc1);
	siftExtractor.compute(image2, keyPoint2, imageDesc2);
	// 4.使用BFMATCHER匹配
	BFMatcher bfmatcher(NORM_L2, true);
	vector<DMatch> bfMatches;
	bfmatcher.match(imageDesc1, imageDesc2, bfMatches);
	// 5.画图
	//Mat bfmatch;
    //drawMatches(image1, keyPoint1, image2, keyPoint2, bfMatches, bfmatch);
    //imshow("bf_match_sift", bfmatch);
	//waitKey(0);
	
	////////////////////////////////////////////////////////
	// ----------- ransac优化匹配结果 ------------//
	
	// 1.在匹配完成后使用得到的匹配点来计算基础矩阵时，
	//   首先需要将特征点对齐，并且将特征点转换为2D点
	//	Align all points
    vector<KeyPoint> alignedKps1, alignedKps2;
    for (size_t i = 0; i < bfMatches.size(); i++) {
        alignedKps1.push_back(keyPoint1[bfMatches[i].queryIdx]);
        alignedKps2.push_back(keyPoint2[bfMatches[i].trainIdx]);
    }

    //Keypoints to points,坐标转换
    vector<Point2f> ps1, ps2;
    for (unsigned i = 0; i < alignedKps1.size(); i++)
        ps1.push_back(alignedKps1[i].pt);
    for (unsigned i = 0; i < alignedKps2.size(); i++)
        ps2.push_back(alignedKps2[i].pt);
	
	// 2.使用RANSAC方法计算基础矩阵后可以得到一个status向量，用来删除错误匹配结果
	vector<uchar> status;
	Mat Fundamental= findFundamentalMat(ps1,ps2,status,FM_RANSAC);
	// 优化匹配结果
	vector<KeyPoint> imgInlier1;
	vector<KeyPoint> imgInlier2;
	vector<DMatch> inlierMatch;
	
	int index = 0;

	for (unsigned i = 0; i < bfMatches.size(); i++){
		if(status[i] != 0){
			imgInlier1.push_back(alignedKps1[i]);
			imgInlier2.push_back(alignedKps2[i]);
			bfMatches[i].trainIdx = index;
			bfMatches[i].queryIdx = index;
			inlierMatch.push_back(bfMatches[i]);
			index++;
		}
	}
	keyPoint1 = imgInlier1;
	keyPoint2 = imgInlier2;
	bfMatches = inlierMatch;

	// 3.计算单应矩阵H，并细化匹配结果
	//Prepare data for findHomography
    vector<Point2f> srcPoints(bfMatches.size());
    vector<Point2f> dstPoints(bfMatches.size());
	 
	for (size_t i = 0; i < bfMatches.size(); i++) {
        srcPoints[i] = keyPoint1[bfMatches[i].trainIdx].pt;
        dstPoints[i] = keyPoint2[bfMatches[i].queryIdx].pt;
    }
	//find homography matrix and get inliers mask
	double reprojectionThreshold = 3;
    vector<uchar> inliersMask(srcPoints.size());
    Mat homography = findHomography(srcPoints, dstPoints, CV_FM_RANSAC, reprojectionThreshold, inliersMask);
	//
	vector<DMatch> inliers;
    for (size_t i = 0; i < inliersMask.size(); i++){
        if (inliersMask[i])
            inliers.push_back(bfMatches[i]);
    }
    bfMatches.swap(inliers);
	Mat bfmatch_ransac;
    drawMatches(image1, keyPoint1, image2, keyPoint2, bfMatches,bfmatch_ransac);
    imshow("bf_match_ransac_sift", bfmatch_ransac);
	//waitKey(0);
	

	return homography;
}