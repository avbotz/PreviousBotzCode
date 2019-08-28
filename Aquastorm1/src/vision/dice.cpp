#include "vision/config.hpp"
#include "mission/config.hpp"
#include "common/log.hpp"


Observation vdice(const cv::Mat &input)
{
	// Returns no observation if the image is invalid.
	if (!input.data)
	{
		// std::clog << "No image data - front camera." << std::endl;
		return Observation(false);
	}

	// Write image to debug so it can be read by server.
	aqualog::image(input, "debug_img.png");

	// TODO Glue Aquastorm to ML. 
	return Observation(0, 0, 0, 0, 1.0, DICE_GOAL);
}

Observation vdiceML(CData *cdata, const cv::Mat &input)
{
	// Returns no observation if the image is invalid.
	if (!input.data)
	{
		// std::clog << "No image data - front camera." << std::endl;
		return Observation(false);
	}

	// Send request to ML dice.
	cdata->message(*cdata->send, input);	

	// Parse reply for location, confidence, and class.
	zmq::message_t rp;
	cdata->send->recv(&rp);
	std::string raw = std::string(static_cast<char*>(rp.data()), rp.size()); 
	std::stringstream strm;
	strm << raw;
	int num; strm >> num;
	switch (num)
	{
		case 0:
			std::clog << "No dice detected." << std::endl;
			return Observation(false);
			break;
		case 1:
			float loc[2];
			float conf;
			float dist;
			int type;
			for (int i = 0; i < 2; i++) strm >> loc[i];
			strm >> conf;
			strm >> dist;
			strm >> type;
			if (std::fabs(loc[0]) < 0.1) return Observation(false);
			Observation obs = Observation(angle::calibrate(loc[0], 640), 0.0, 0.0, dist, 1.0, DICE_GOAL);
			std::clog << obs << std::endl;
			return obs;
			break;
	}

	// TODO Glue Aquastorm to ML. 
	return Observation(false);
}

