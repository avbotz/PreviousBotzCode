#ifndef INTERFACE_IDATA_HPP
#define INTERFACE_IDATA_HPP

#include <mutex>

#include <opencv2/core/core.hpp>

class IData
{
	public:
		IData();	
		void lock();
		void unlock();

		cv::Mat front_img();
		cv::Mat down_img();
		void set_front_img(cv::Mat);
		void set_down_img(cv::Mat);

	private:
		cv::Mat front, down;
		std::mutex mtx;
};

#endif
