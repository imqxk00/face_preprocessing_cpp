#include "data_processing.h"

int perImgProcessing(const Mat img, const string faceDetectModel, Mat result)
{
	FACE_HANDLE face_detection_handle;
	int ret_fd_init = getDetectionHandle(&face_detection_handle, faceDetectModel);
	if(0 != ret_fd_init || NULL == &face_detection_handle) {
		printf("Error init for face alignment!\n");
		return -1;
	}

	vecR rect;
	string error;
	int ret_fd = detectFace(face_detection_handle, img, rect, error);

	return 0;
}