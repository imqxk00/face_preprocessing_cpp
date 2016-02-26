#include "data_processing.h"


int test1()
{
	const Mat img =  imread("D:\\code\\cpp\\face_preprocessing_cpp\\model\\1.jpg");
	const string faceDetectModel = "D:\\code\\cpp\\face_preprocessing_cpp\\model\\haarcascade_frontalface_alt2.xml";
	const string detectionModelPath = "D:\\code\\cpp\\face_preprocessing_cpp\\model\\DetectionModel-v1.5.bin";
	const string trackingModelPath = "D:\\code\\cpp\\face_preprocessing_cpp\\model\\TrackingModel-v1.10.bin";
	const float ec_mc_y = 48.0;
	const float ec_y = 40;
	vecM result;

	perImgProcessing(img, faceDetectModel, detectionModelPath, trackingModelPath, ec_mc_y, ec_y, result);

	return 0;
}

int main()
{
	test1();
	return 0;
}