#include "face_detection.h"
#include "face_alignment.h"


int perImgProcessing(const Mat img, const string faceDetectModel, const string detectionModelPath, 
					 const string trackingModelPath, const float ec_mc_y, const float ec_y, vecM &result);