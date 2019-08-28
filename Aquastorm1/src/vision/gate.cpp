#include "vision/config.hpp"
#include "vision/filters.hpp"
#include "mission/config.hpp"
#include "common/log.hpp"


Observation vgate(const cv::Mat &input)
{
	// Returns no observation if the image is invalid.
	if (!input.data)
		return Observation(false);

	// Illuminate the image.
	cv::Mat illum = illumination(input);

	// Light blur.
	cv::Mat blur;
	cv::blur(illum, blur, cv::Size(5, 5));

	// Canny edge detection.
	cv::Mat can, cdst;
    cv::Canny(blur, can, 150, 200, 3);
    cv::cvtColor(can, cdst, cv::COLOR_GRAY2BGR);
	
    // Get lines with HoughLinesP
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(can, lines, 1, CV_PI/180, 35, 20, 20);
    for (int i = 0; i < lines.size(); i++) 
    {
        cv::Vec4i line = lines[i];
		int x1=line[0],y1=line[1],x2=line[2],y2=line[3];
    	float dist = std::sqrt(std::pow(std::abs(x1 - x2), 2) + std::pow(std::abs(y1 - y2), 2));
    	float rotation = std::acos(std::abs(y1 - y2) / dist) * 180/CV_PI;
        if (std::abs(rotation) <= 15)
		{
            // solutions.push_back(solution); 
            // solution.print();
            cv::line(cdst, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 255), 3, CV_AA);		
			int midpoint = (x1+x2)/2;
			float yaw = angle::calibrate(midpoint, 640);
			aqualog::image(cdst, 'e');

			return Observation(yaw, 0.0, 0.0, 0.0, 0.0, GATE_GOAL);
        }
    }
	
	// TODO Glue Aquastorm to ML. 
	return Observation(false);
}
