#include "control/connect.hpp"
#include "interface/idata.hpp"


IData::IData()
{
	// Default constructor, not needed atm.
}

cv::Mat IData::front_img()
{
	return this->front;	
}

cv::Mat IData::down_img()
{
	return this->down;
}

void IData::set_front_img(cv::Mat _front)
{
	this->front = _front;
}

void IData::set_down_img(cv::Mat _down) 
{
	this->down = _down;
}

void IData::lock() 
{
	this->mtx.lock();
}

void IData::unlock()
{
	this->mtx.unlock();
}
