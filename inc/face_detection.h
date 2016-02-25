// face_detection.h : include file for face detection, which contains opencv 
// method and Shiqi Yu's method(https://github.com/ShiqiYu/libfacedetection)
//
#pragma once

#include "stdafx.h"

#define OPENCV 1

#if OPENCV
	int getDetectionHandle(FACE_HANDLE* handle, const string modelPath);
	int detectFace(FACE_HANDLE handle, const Mat img, vecR &rect, string &error);
	int  releaseDetectionHandle(FACE_HANDLE* handle);
#else
	#include <facedetect-dll.h>

#endif

