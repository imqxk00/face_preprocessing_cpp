#include "data_processing.h"

int perImgProcessing(const Mat img, const string faceDetectModel, const string detectionModelPath, 
					 const string trackingModelPath, const float ec_mc_y, const float ec_y, vecM &result)
{

	// face detection

#if OPENCV

	FACE_HANDLE face_detection_handle;
	int ret_fd_init = getDetectionHandle(&face_detection_handle, faceDetectModel);
	if(0 != ret_fd_init || NULL == &face_detection_handle) {
		cout << "Error init for face alignment!" << endl;
		return -1;
	}

	vecR rect;
	string error;
	int ret_fd = detectFace(face_detection_handle, img, rect, error);

	if(0 != ret_fd)
	{
		cout << error <<endl;
		return -1;
	}
#else


#endif

	// face alignment
	FACE_HANDLE face_alignment_handle;
	int ret_fa = getAlignmentHandle(&face_alignment_handle, detectionModelPath, trackingModelPath);
	if(0 != ret_fa || NULL == face_alignment_handle) {
		cout << "Error init for face alignment!" << endl;
		return -1;
	}

	for(int i = 0; i < rect.size(); ++i)
	{
		Mat temp;
		faceAlignment(face_alignment_handle, img, rect[i], ec_mc_y, ec_y, temp, error);
		/*if(0 != ret)
		{
			cout << error << endl;
			continue;
		}*/	
		result.push_back(temp);
	}

	releaseDetectionHandle(&face_detection_handle);
	releaseAlignmentHandle(&face_alignment_handle);

	return 0;
}