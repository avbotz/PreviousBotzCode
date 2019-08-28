#ifndef INTERFACE_NDATA_HPP
#define INTERFACE_NDATA_HPP

#include <mutex>

#include "common/state.hpp"
#include "common/observation.hpp"
#include "control/connect.hpp"

class NData
{
	public:
		State previous, state, desired;
		Observation observation;
		bool alive;

		NData();
		void lock();
		void unlock();

	private:
		std::mutex mtx;
};

#endif
