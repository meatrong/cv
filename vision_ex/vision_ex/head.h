#ifndef HEAD_H//���ã���ֹgraphics.h���ظ�����
#define HEAD_H

#include "highgui/highgui.hpp"    
#include "opencv2/nonfree/nonfree.hpp"    
#include "opencv2/legacy/legacy.hpp" 
#include "opencv2/core/core.hpp"
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>  
#include <stdio.h>
#include <Windows.h>
#include <time.h>  
#include <math.h>

using namespace cv;
using namespace std;


int matchone(Mat src_image, Mat test_image);
//Mat match(Mat image01);
int extract();

Mat resizeone(Mat image,int ratio);
Mat equal(Mat imageS, Mat imageL);
Mat transform(Mat img);
int debug();
void onMouse(int event,int x,int y,int flags,void *utsc);
int clickcorrect();
void CalcCorners(const Mat& H, const Mat& src);
void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst);

Mat bfmatch(Mat image_src, Mat image_test);
int kmatch(Mat imageDesc1, Mat imageDesc0);
int stitch(Mat image01, Mat image02);
int persadjust(Mat image01, Mat image02);
//int predata();
typedef struct
{
    Point2f left_top;
    Point2f left_bottom;
    Point2f right_top;
    Point2f right_bottom;
}four_corners_t;


#define num 8
#define DST_IMG_WIDTH 550        //��Ҫ����ͼƬ��ĳߴ���
#define SRC_IMG_HEIGH 550        //��Ҫ����ͼƬ��ĳߴ�߶�


//./test/5.jpg -- 200
// 7--200 -> У������ ��������Ϊͼ��̫С�� �ĳ�550�Ժ�Ϳ���У����

#endif