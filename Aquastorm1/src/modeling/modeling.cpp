#include <thread>
#include <chrono>

#include "common/log.hpp"
#include "modeling/modeling.hpp"
#include "modeling/mcl.hpp"


void modeling(NData *data)
{
	std::clog << "modeling" << std::endl;

	// Initialize particles struct.
	MCL mcl;
	mcl.init(data);

	while (true)
	{
		mcl.m_update();
		mcl.s_update();
		mcl.resample();

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	std::cerr << "ERROR: Completed modeling." << std::endl;
	aqualog::text("ERROR: Completed modeling.");
}
