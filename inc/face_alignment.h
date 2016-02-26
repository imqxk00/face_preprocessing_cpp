// face_alignment.h : include file for face alignment, which the facial 
// point detection is by SDM. 
//
#pragma once

#include "stdafx.h"
#include <intraface\FaceAlignment.h>

using namespace INTRAFACE;

int getAlignmentHandle(FACE_HANDLE* handle, const string detectionModelPath, 
					   const string trackingModelPath);
int faceAlignment(FACE_HANDLE handle, const Mat img, const Rect rect, 
				  const float ec_mc_y, const float ec_y, Mat &result,
				  string &error);
int  releaseAlignmentHandle(FACE_HANDLE* handle);
