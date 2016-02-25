#include "data_processing.h"


int test1()
{
	Mat img =  imread("D:\\code\\cpp\\face_preprocessing_cpp\\model\\1.jpg");
	string faceDetectModel = "D:\\code\\cpp\\face_preprocessing_cpp\\model\\haarcascade_frontalface_alt2.xml";
	Mat result;

	perImgProcessing(img, faceDetectModel, result);

	return 0;
}

int main()
{
	test1();
	return 0;
}