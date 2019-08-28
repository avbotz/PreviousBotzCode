#ifndef VISION_FILTERS_HPP
#define VISION_FILTERS_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat equalize_hist(bool, bool, bool, const cv::Mat &);
cv::Mat illumination(const cv::Mat &);
cv::Mat homomorphic(const cv::Mat &);
cv::Mat butterworth(const cv::Mat &, int, int, int, int);
void fft(const cv::Mat &, cv::Mat &);

#endif
