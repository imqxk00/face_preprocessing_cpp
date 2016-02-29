// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <assert.h>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <time.h>
#include <fstream>
#include <random>
#include <atlstr.h>
#include <atltypes.h>
#include <omp.h>
#include <strstream>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

typedef void* FACE_HANDLE;
typedef vector<Rect> vecR;
typedef vector<Mat> vecM;
typedef vector<string> vecS;




/*******************************************
			File IO Processing
********************************************/
#include "stdafx.h"
#include <direct.h>
#include <io.h>

int create_directory(const char *directory);
int get_dir_from_filename(const char *file_name, char *dir);
int replace_ext_name(char *id, const char *ext);
