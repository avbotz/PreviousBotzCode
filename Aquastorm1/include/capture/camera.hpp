#ifndef CAPTURE_CAMERA_HPP
#define CAPTURE_CAMERA_HPP

#include <opencv2/core/core.hpp>
#include <flycapture/FlyCapture2.h>

struct Camera 
{
	FILE* in = stdin;
	FILE* out = stdout;
	FILE* log = stderr;
	unsigned int numCameras;
	unsigned int idx;
	FlyCapture2::PGRGuid guid;
	FlyCapture2::Camera cam;

	bool init(unsigned int);
	bool quit();
	void flipImage(cv::Mat&);
	cv::Mat capture(bool);
};

#endif
