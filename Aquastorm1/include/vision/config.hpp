#ifndef INCLUDE_VISION_HPP
#define INCLUDE_VISION_HPP 

#include <zmq.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "interface/cdata.hpp"
#include "common/observation.hpp"
#include "common/angle.hpp"

static float FOV = 120.0f;

Observation vgate(const cv::Mat &);
Observation vdice(const cv::Mat &);
Observation vdiceML(CData*, const cv::Mat &);

#endif 	
