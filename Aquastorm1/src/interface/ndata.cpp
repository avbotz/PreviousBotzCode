#include "control/connect.hpp"
#include "interface/ndata.hpp"


NData::NData()
{
	State temp = atmega::read_state();
	this->previous = temp;
	this->state = temp;
	this->desired = temp;
}

void NData::lock() 
{
	this->mtx.lock();
}

void NData::unlock()
{
	this->mtx.unlock();
}
