#include <iostream>
#include <thread>
#include <chrono>

#include "common/log.hpp"
#include "common/config.hpp"
#include "control/connect.hpp"
#include "control/control.hpp"


/*
 * Handles submarine movement using commands from mission.
 */
void control(NData *data)
{
	std::clog << "control" << std::endl;

	// Set default power.
	atmega::cmd::set_power(POWER_LEVEL);

	// Update atmega and data mutex.
	while (true)
	{
		// Update current state to queried state and kill state.
		data->lock();
		data->state = atmega::read_state();
		data->alive = atmega::read_alive();
		data->unlock();

		// std::clog << "q " << data->state << std::endl;
		// std::clog << "d " << data->desired << std::endl;
		// aqualog::text("q " + data->state.to_text());
	
		/*
		 * TODO Write all the sub-specific commands (drop, grab, shoot, etc).
		 */

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	std::cerr << "ERROR: Completed control." << std::endl;
	aqualog::text("ERROR: Completed control.");
}

