#ifndef INTERFACE_CDATA_HPP
#define INTERFACE_CDATA_HPP

#include <iostream>
#include <mutex>

#include <zmq.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "server/base64.h"

class CData
{
	public:
		zmq::socket_t *send;
		zmq::socket_t *recv;
		Base64 b64;

		CData(zmq::socket_t*, zmq::socket_t*);
		void message(zmq::socket_t &, const std::string &);
		void message(zmq::socket_t &, const cv::Mat &);
		void lock();
		void unlock();

	private:
		std::mutex mtx;
};

#endif
