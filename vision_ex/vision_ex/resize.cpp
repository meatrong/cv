#include "head.h"

Mat resizeone(Mat image,int ratio)
{
	Mat image01;
	//cout << image.rows << " " << image.cols << endl;
	if(image.rows > ratio){
		double fx = (double)ratio / (double)image.rows;
		resize(image,image01 ,Size(), fx, fx,INTER_LINEAR);	
		image = image01.clone();

	}
	else if(image.cols > ratio){
		double fy = (double)ratio / (double)image.cols;
		//cout << fy << endl;
		resize(image,image01 ,Size(),fy, fy, INTER_LINEAR);
		image = image01.clone();
	}

	//cout << "rsize:" << image01.rows << " " << image01.cols << endl;

	return image;
}
Mat equal(Mat imageS, Mat imageL){
	Mat imgTemp;
	double fx = (double) imageL.cols / ((double)imageL.rows / (double)imageS.rows);
	//resize(imageL,imgTemp ,Size(), fx, imageS.rows, INTER_LINEAR);
	resize(imageL, imgTemp, imageS.size(),0,0)	;
	imageL = imgTemp.clone();
		return imageL;
}