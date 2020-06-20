#include "head.h"

extern four_corners_t corners;

int main()    
{
	int funcnum ;	// ����ѡ��
	Mat  image01, image02;
	while(1){
	cout << "��ѡ���ܣ�1.������ѡȡ��2.����ƥ�䣬3.͸��У����4.ͼƬƴ�ӣ�5.�ֶ�У����6.�˳�" << endl;
	cin >> funcnum;
	while(funcnum > 6 || funcnum < 0){
		cout << "�������������������ȷ��ţ�" << endl;
		cin >> funcnum;
	}
	switch (funcnum){
	case 0:
		cout << "����debugģʽ" << endl;
		debug();
		return 0;
		break;
	case 1 :
		cout << "��ѡ������������ȡ" << endl;
		image01 = imread("./data/ucasmap1.jpg", 1);
		image02 = imread("./data/ucasmap2.jpg", 1);
		extract();
		break;
	case 2:{
		cout << "��ѡ��������ƥ��" << endl;
		image01 = imread("./data/test/ucasmap2.jpg", 1);
		image02 = imread("./data/test/ucasmap1.jpg", 1);
		
		image02 = transform(image02);
		Mat bfhomo = bfmatch(image01, image02);
		
		break;
		   }
	case 3:
		{
		cout << "��ѡ����͸�ӱ任" << endl;
		image01 = imread("./data/test/match1.jpg", 1);
		image02 = imread("./data/test/match2.jpg", 1);
		image02 = transform(image02);
		//bfmatch(image01, image02);
		persadjust(image01, image02);
		/*Mat bfhomo = bfmatch(image01, image02);
		cout<<"�任����Ϊ��\n" << bfhomo << endl << endl; //���ӳ�����      
		double bfadjustValue=image01.cols;  
		Mat bfadjustMat=(Mat_<double>(3,3)<<1.0,0,35,0,1.0,65,0,0,1.0);  
		cout<<"��������Ϊ��\n"<<  bfadjustMat << endl<<endl;  
		cout<<"������任����Ϊ��\n"<< bfadjustMat*bfhomo << endl;  
  
		//ͼ����׼  
		Mat bfimageTransform1,bfimageTransform2;  
		warpPerspective(image02,bfimageTransform1,bfhomo,Size(image01.cols,image01.rows));  
		warpPerspective(image02, bfimageTransform2, bfadjustMat*bfhomo, Size(image01.cols*1.3,image01.rows*1.8));
	
		imshow("bf_ransac_dst",bfimageTransform1);
		*/

		break;
		   }
	case 4:
		cout << "��ѡ����ͼƬƴ��" << endl;
		image01 = imread("./data/test/stitch1.jpg", 1);
	image02 = imread("./data/test/stitch3.jpg", 1);
		stitch(image01, image02);
		break;
	case 5:
		cout << "��ѡ�����ֶ�У��ͼƬ" << endl;
		cout << "����ͼƬ�����ε���ĸ�����Ϊ�ĸ�����" << endl;
		clickcorrect();
			break;

	case 6:
		cout << "��лʹ�ã�" << endl;
		return 0;
		break;
	}
	waitKey();  
	}

    return 0;    
}  
