#include "face_alignment.h"

//-------------------------------------------
//		  Rotation Tranformation 
//-------------------------------------------
Point2f getRotationLoc(double angle, Point2f src_loc, Point2f src_center, Point2f dst_center){
	Point2f src=src_loc  -src_center;
	float dst_x =src.x*cos(angle) + src.y*sin(angle) + dst_center.x;
	float dst_y = -src.x*sin(angle) + src.y*cos(angle) + dst_center.y;

	return Point2f(dst_x,dst_y);
}

//-------------------------------------------
//		Crop Face According to Landmark
//-------------------------------------------
int imgCrop(const Mat& src, Mat& dst, Rect rect){
	dst = cv::Mat::zeros(rect.height,rect.width,CV_8UC1);

	Rect rect_src(rect);
	Rect rect_dst(0,0,rect.width,rect.height);

	if(rect.x<0){
		rect_dst.x = 0-rect.x;
		rect_src.x = 0;
		rect_src.width += rect.x;

	}
	if(rect.y<0){
		rect_dst.y = 0-rect.y;
		rect_src.y = 0;
		rect_src.height += rect.y;
	}
	if(rect.x+rect.width > src.cols){
		rect_src.width -= (rect.x+rect.width - src.cols);
	}
	if(rect.y+rect.height>src.rows){
		rect_src.height -= (rect.y + rect.height - src.rows);
	}
	rect_dst.width = rect_src.width;
	rect_dst.height = rect_src.height;

	
	Mat src_sub = src(rect_src);
	Mat dst_sub = dst(rect_dst);
	src_sub.copyTo(dst_sub);

	return 0;
}

//-------------------------------------------
//			 Face Alignment
//-------------------------------------------
Mat alignment(Mat img, Mat landmark, const float ec_mc_y, const float ec_y){

	float eyeLX, eyeLY, eyeRX, eyeRY, eCenterX,eCenterY, mCenterX,mCenterY;
	eyeLX = (landmark.at<float>(0, 19)+landmark.at<float>(0, 20)+landmark.at<float>(0, 21)+landmark.at<float>(0, 22)
		+landmark.at<float>(0, 23)+landmark.at<float>(0, 24))/6;
	eyeLY = (landmark.at<float>(1, 19)+landmark.at<float>(1, 20)+landmark.at<float>(1, 21)+landmark.at<float>(1, 22)
		+landmark.at<float>(1, 23)+landmark.at<float>(1, 24))/6;

	eyeRX = (landmark.at<float>(0, 25)+landmark.at<float>(0, 26)+landmark.at<float>(0, 27)+landmark.at<float>(0, 28)
		+landmark.at<float>(0, 29)+landmark.at<float>(0, 30))/6;
	eyeRY = (landmark.at<float>(1, 25)+landmark.at<float>(1, 26)+landmark.at<float>(1, 27)+landmark.at<float>(1, 28)
		+landmark.at<float>(1, 29)+landmark.at<float>(1, 30))/6;

	mCenterX = (landmark.at<float>(0, 43)+landmark.at<float>(0, 44)+landmark.at<float>(0, 45)+landmark.at<float>(0, 46)
		+landmark.at<float>(0, 47)+landmark.at<float>(0, 48))/6;
	mCenterY = (landmark.at<float>(1, 43)+landmark.at<float>(1, 44)+landmark.at<float>(1, 45)+landmark.at<float>(1, 46)
		+landmark.at<float>(1, 47)+landmark.at<float>(1, 48))/6;

	eCenterX = (eyeLX+eyeRX)/2;
	eCenterY = (eyeLY+eyeRY)/2;

	float angTan = (eyeLY - eyeRY)/(eyeLX - eyeRX);
	float angle = atan(angTan)/PI * 180;

	//rotate src img according to locations of two eyes
	Point2f src_center(img.cols/2.0F, img.rows/2.0F);
	Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
	Mat dst;
	warpAffine(img, dst, rot_mat, img.size());
	Point2f dst_center(dst.cols/2.0F, dst.rows/2.0F);

	//compute bbox of the dst img
	Point2f dst_ec = getRotationLoc(angle*CV_PI/180, Point2f(eCenterX,eCenterY), src_center,dst_center);
	Point2f dst_mc = getRotationLoc(angle*CV_PI/180, Point2f(mCenterX,mCenterY), src_center,dst_center);

	Mat result;

	float dist_ec_mc = abs(dst_mc.y - dst_ec.y);
	float scale = ec_mc_y / dist_ec_mc;
	float dist_y = ec_y / scale;
	float height = dist_y*2 + dist_ec_mc;

	Rect rect(dst_ec.x-height/2, dst_ec.y - dist_y, height, height); 
	
	imgCrop(dst, result, rect);

	return result;
}


int getAlignmentHandle(FACE_HANDLE* handle, const string detectionModelPath, 
					   const string trackingModelPath)
{
	static XXDescriptor pXXD(4);
	FaceAlignment* face_alignment = new FaceAlignment(detectionModelPath.c_str(),
													  trackingModelPath.c_str(),
													  &pXXD);
	if(!face_alignment->Initialized())
	{
		cout << "Face Alignment Handle Init Error.."<< endl;
		return -1;
	}
	(*handle) = (FACE_HANDLE)face_alignment;
	return 0;
}


int faceAlignment(FACE_HANDLE handle, const Mat img, const Rect rect, 
				  const float ec_mc_y, const float ec_y, Mat &result,
				  string &error)
{
	FaceAlignment* face_alignment = (FaceAlignment*)handle;
	Mat landmark;
	float score;
	
	if(face_alignment->Detect(img, rect, landmark, score) == IF_OK && score > 0.5)
	{
		result = alignment(img, landmark, ec_mc_y, ec_y);
	}
	else
	{
		error = "0203";
		result = img(rect);
	}

	return 0;
}

int releaseAlignmentHandle(FACE_HANDLE* handle)
{
	FaceAlignment* face_alignment = (FaceAlignment*)(*handle);
	if(face_alignment)
		delete face_alignment;
	face_alignment = NULL;
	return 0;
}