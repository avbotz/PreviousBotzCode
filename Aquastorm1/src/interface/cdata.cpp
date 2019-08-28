#include "interface/cdata.hpp"


CData::CData(zmq::socket_t *s, zmq::socket_t *r)
{
	this->send = s;
	this->recv = r;
	b64 = Base64();
}

void CData::message(zmq::socket_t &socket, const std::string &message)
{
	// Send message over the socket.
	zmq::message_t rep(message.size());
	memcpy((void*) rep.data(), message.data(), message.size());
	socket.send(rep);
}

void CData::message(zmq::socket_t &socket, const cv::Mat &img)
{
	// Encode image so it can be sent over ZMQ .
	std::vector<uchar> jpeg;
	std::vector<int> params;
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	params.push_back(30);
	cv::imencode(".jpg", img, jpeg, params);
	std::string data = std::string(reinterpret_cast<char*>(jpeg.data()), jpeg.size());
	std::string encoded;
	b64.Encode(data, &encoded);

	// Send message over the socket. 
	zmq::message_t rep(encoded.size());
	memcpy((void*) rep.data(), encoded.data(), encoded.size());
	socket.send(rep);
}

void CData::lock() 
{
	this->mtx.lock();
}

void CData::unlock()
{
	this->mtx.unlock();
}
