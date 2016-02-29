#include "data_processing.h"
#include <fstream>

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

int batchProcessing(const string rootPath, const string listPath, const string faceDetectModel, const string detectionModelPath, 
					const string trackingModelPath, const float ec_mc_y, const float ec_y, const string saveRootPath, const int size)
{
	ofstream logfile("log.txt");
	ifstream infile(listPath);
	string filename;
	vecS lines;

	while(infile >> filename)
		lines.push_back(filename);
	infile.close();
	cout << "A total of " << lines.size() << " images." << endl;


	string str;
	Mat img;

	FACE_HANDLE face_detection_handle;
	int ret_fd_init = getDetectionHandle(&face_detection_handle, faceDetectModel);
	if(0 != ret_fd_init || NULL == &face_detection_handle) {
		cout << "Error init for face alignment!" << endl;
		return -1;
	}
	vecR rect;
	string error;

	FACE_HANDLE face_alignment_handle;
	int ret_fa = getAlignmentHandle(&face_alignment_handle, detectionModelPath, trackingModelPath);
	if(0 != ret_fa || NULL == face_alignment_handle) {
		cout << "Error init for face alignment!" << endl;
		return -1;
	}

	for(int i = 0; i < lines.size(); ++i)
	{
		rect.clear();
		str = rootPath + lines[i];
		img = imread(str, CV_LOAD_IMAGE_GRAYSCALE);
		int ret_fd = detectFace(face_detection_handle, img, rect, error);
		if(0 != ret_fd)
		{
			cout << "The " << i << " image has "<< error <<endl;
			logfile << lines[i] << " [Face Detection Error]." << endl;
			continue;
		}
		for(int j = 0; j < rect.size(); ++j)
		{
			Mat result;
			int ret_fa = faceAlignment(face_alignment_handle, img, rect[j], ec_mc_y, ec_y, result, error);
			if(0 != ret_fa)
			{
				cout << "The " << i << " image has "<< error <<endl;
				logfile << lines[i] << " [Facial Points Detection Error]." << endl;
				continue;
			}
			resize(result, result, Size(size, size));
			string savePath;
			stringstream count;
			char dir[1024];
			//sprintf(savePath, "%s%s_%d.jpg", saveRootPath.c_str(), lines[i].c_str(), j);
			count << j;
			savePath = saveRootPath + lines[i] + "_" + count.str() + ".jpg";
			get_dir_from_filename(savePath.c_str(), dir);
			int ret = create_directory(dir);
			if(0 != ret)
			{
				cout << savePath << " create file error!" << endl;
				logfile << lines[i] << " [IO Error]." << endl;
			}
			else
				cout << savePath << " is finished." << endl;
			imwrite(savePath, result);

		}
	}
	logfile.close();
	releaseDetectionHandle(&face_detection_handle);
	releaseAlignmentHandle(&face_alignment_handle);
}