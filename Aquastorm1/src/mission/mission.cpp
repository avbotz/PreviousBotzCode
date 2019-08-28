#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include "mission/mission.hpp"
#include "mission/config.hpp"
#include "control/connect.hpp"
#include "common/config.hpp"
#include "common/log.hpp"
#include "server/base64.h"


/*
 * Handles task planning and route planning.
 */
void mission(IData *idata, NData *ndata, CData *cdata)
{
	std::clog << "mission" << std::endl;

	// Keep a list of visited nodes. Unused at the moment. 
	// std::vector<bool> visited(goals.size());

	// For static pathing, just keep track of the current node. 
	int current = 0;

	while (true) 
	{
		// Wait until sub is unkilled.
		/*
		while (!atmega::read_alive()) 
		{
			std::clog << "Is killed, mission not running." << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		*/

		if (MISSION_MODE == MissionMode::DEADRECKONING)
		{
			State s1(4, 1, -0.6, ndata->state.yaw, 0, 0);
			State s2(4, -1, -0.6, ndata->state.yaw, 0, 0);
			State s3(0, 0, -0.6, ndata->state.yaw, 0, 0);
			deadreckon(ndata, s1);
			deadreckon(ndata, s2);
			deadreckon(ndata, s3);
		}

		// TODO Find a better pathing system.
		// Right now, the pathing is set to do static. Preferably, it would take
		// into account aspects like points, TTC (time to completion), etc.
		Goal goal = goals[current];	
		goal.run(idata, ndata, cdata);

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	std::cerr << "ERROR: Completed mission." << std::endl;
	aqualog::text("ERROR: Completed mission.");
}

void deadreckon(NData *ndata, State state)
{
	atmega::write(state);
	while (std::fabs(ndata->state.x - state.x) > 0.1 || std::fabs(ndata->state.y - state.y) > 0.1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	}
}
