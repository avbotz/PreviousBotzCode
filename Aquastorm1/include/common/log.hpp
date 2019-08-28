#ifndef COMMON_LOG_HPP
#define COMMON_LOG_HPP 

#include <string>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/*
 * Log is a keyword, and this should be wrapped in a namespace.
 * Use 'f' for front camera, 'd' for down camera, 'e' for debug!
 */
namespace aqualog
{
	extern std::string filename;
    extern std::vector<std::string> pending;
	void image(const cv::Mat&, char);
	void image(const cv::Mat&, std::string);
	void init();
	void text(std::string);
}

#endif
