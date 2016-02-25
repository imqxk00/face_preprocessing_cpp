// face_detection.cpp : cpp file for face detection, which contains opencv 
// method and Shiqi Yu's method(https://github.com/ShiqiYu/libfacedetection)
//
#include "face_detection.h"


#if OPENCV
/*******************************************
		Get Face Detection Handle
********************************************/
int getDetectionHandle(FACE_HANDLE* handle, const string modelPath)
{
	CascadeClassifier* face_cascade = new CascadeClassifier(modelPath);
	if(!face_cascade->load(modelPath))
	{
		cout << "Face Detection Handle Init Error.."<< endl;
		return -1;
	}
	(*handle) = (FACE_HANDLE)face_cascade;
	return 0;
}

/*******************************************
			Face Detection 
********************************************/
int detectFace(FACE_HANDLE handle, const Mat img, vecR &rect, string &error)
{
	const int width = img.cols;
	const int height = img.rows;
	const int channel = img.channels();
	CascadeClassifier* face_cascade = (CascadeClassifier*)handle;
	Mat imgGray;
	
	if(width < 50 || height < 50)
	{
		error = "0201";
		return -1;
	}
	
	if(1 != channel)
		cvtColor(img, imgGray, CV_BGR2GRAY);
	else
		imgGray = img;

	face_cascade->detectMultiScale(imgGray, rect, 1.2, 2, 0, cv::Size(50, 50));

	if(0 == rect.size())
	{
		error = "0202";
		return -2;
	}

	return 0;
}

/*******************************************
			Release Handle
********************************************/
int  releaseDetectionHandle(FACE_HANDLE* handle)
{
	CascadeClassifier* face_cascade = (CascadeClassifier*)handle;
	if(face_cascade)
		delete face_cascade;
	face_cascade = NULL;
	return 0;
}


#else



#endif