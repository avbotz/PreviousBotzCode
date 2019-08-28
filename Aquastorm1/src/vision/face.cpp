#include "common/log.hpp"
#include "vision/face.hpp"


Observation face(const cv::Mat &input)
{
	// Catch empty images in beginning.
	if (!input.data)
		return Observation(false);

	cv::CascadeClassifier face_cascade;
	face_cascade.load(face_cascade_name);

	cv::Mat gray, output;
	cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(gray, gray);
	output = input.clone();

	std::vector<cv::Rect> faces;
	face_cascade.detectMultiScale(input, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

	// Faces need to exist, otherwise return blank observation.
	// TODO Find a better method of handling no observation.
	if (faces.size() > 0) 
	{
		// Calculate center and draw ellipses.
		cv::Point center(faces[0].x + faces[0].width*0.5, faces[0].y + faces[0].height*0.5);
		cv::ellipse(output, center, cv::Size(faces[0].width*0.5, faces[0].height*0.5), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);

		// Calculate yaw to return.
		float yaw = std::atan2((input.cols - faces[0].x), 1.0);
		aqualog::image(output, 'e');

		// return Observation(yaw, 0.0, 0.0, 0.0, 0.0);
	}
	else 
	{
		return Observation(false);
	}
}
