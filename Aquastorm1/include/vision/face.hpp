#ifndef VISION_FACE_HPP 
#define VISION_FACE_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "common/observation.hpp"

static const cv::String face_cascade_name = "face.xml";

Observation face(const cv::Mat &);

#endif
