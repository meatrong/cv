#include "head.h"

extern four_corners_t corners;

int main()    
{
	int funcnum ;	// 功能选择
	Mat  image01, image02;
	while(1){
	cout << "请选择功能：1.特征点选取，2.特征匹配，3.透视校正，4.图片拼接，5.手动校正，6.退出" << endl;
	cin >> funcnum;
	while(funcnum > 6 || funcnum < 0){
		cout << "输入错误！请重新输入正确编号：" << endl;
		cin >> funcnum;
	}
	switch (funcnum){
	case 0:
		cout << "进入debug模式" << endl;
		debug();
		return 0;
		break;
	case 1 :
		cout << "您选择了特征点提取" << endl;
		image01 = imread("./data/ucasmap1.jpg", 1);
		image02 = imread("./data/ucasmap2.jpg", 1);
		extract();
		break;
	case 2:{
		cout << "您选择了特征匹配" << endl;
		image01 = imread("./data/test/ucasmap2.jpg", 1);
		image02 = imread("./data/test/ucasmap1.jpg", 1);
		
		image02 = transform(image02);
		Mat bfhomo = bfmatch(image01, image02);
		
		break;
		   }
	case 3:
		{
		cout << "您选择了透视变换" << endl;
		image01 = imread("./data/test/match1.jpg", 1);
		image02 = imread("./data/test/match2.jpg", 1);
		image02 = transform(image02);
		//bfmatch(image01, image02);
		persadjust(image01, image02);
		/*Mat bfhomo = bfmatch(image01, image02);
		cout<<"变换矩阵为：\n" << bfhomo << endl << endl; //输出映射矩阵      
		double bfadjustValue=image01.cols;  
		Mat bfadjustMat=(Mat_<double>(3,3)<<1.0,0,35,0,1.0,65,0,0,1.0);  
		cout<<"调整矩阵为：\n"<<  bfadjustMat << endl<<endl;  
		cout<<"调整后变换矩阵为：\n"<< bfadjustMat*bfhomo << endl;  
  
		//图像配准  
		Mat bfimageTransform1,bfimageTransform2;  
		warpPerspective(image02,bfimageTransform1,bfhomo,Size(image01.cols,image01.rows));  
		warpPerspective(image02, bfimageTransform2, bfadjustMat*bfhomo, Size(image01.cols*1.3,image01.rows*1.8));
	
		imshow("bf_ransac_dst",bfimageTransform1);
		*/

		break;
		   }
	case 4:
		cout << "您选择了图片拼接" << endl;
		image01 = imread("./data/test/stitch1.jpg", 1);
	image02 = imread("./data/test/stitch3.jpg", 1);
		stitch(image01, image02);
		break;
	case 5:
		cout << "您选择了手动校正图片" << endl;
		cout << "请在图片上依次点击四个点作为四个顶点" << endl;
		clickcorrect();
			break;

	case 6:
		cout << "感谢使用！" << endl;
		return 0;
		break;
	}
	waitKey();  
	}

    return 0;    
}  
