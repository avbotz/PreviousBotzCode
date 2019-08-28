#include "common/log.hpp"
#include "vision/test.hpp"
#include "mission/config.hpp"
#include "vision/filters.hpp"


Observation test(const cv::Mat &input)
{
	// TODO There are sometimes no images in the beginning, need better handling.
	if (!input.data)
		return Observation(false);

	// Increase contrast and denoise.
	cv::Mat filter = homomorphic(input);
	aqualog::image(filter, 'e');

	return Observation(0.0, 0.0, 0.0, 0.0, 0.5, GATE_GOAL);
}
