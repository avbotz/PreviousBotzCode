#include <cmath>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "control/connect.hpp"
#include "common/angle.hpp"
#include "mission/dice.hpp"
#include "server/base64.h"
#include "vision/config.hpp"


State calculate(Observation obs, const State &orig)
{
	// Setup desired state.
	State desired = orig;

	// Ensure that observation returned a positive.
	// Detemine a new desired state.
	if (obs.valid)	
		if (std::fabs(obs.y) < 5.0f)
			desired.x += 1.0f;
		else 
			desired.yaw = angle::add(desired.yaw, obs.y);
	else 
	{
		// Move to write state.
		if (std::fabs(orig.x - dice::model.x) > 0.1 || std::fabs(orig.y - dice::model.y) > 0.1)
		{
			desired.x = dice::model.x;
			desired.y = dice::model.y;
			desired.z = dice::model.z;
		}

		// Otherwise, run general scan for task. 
		else
		{
			desired.yaw = angle::add(desired.yaw, 18.0f);
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		}
	}

	return desired;
}

void dice::run(IData *idata, NData *ndata, CData *cdata)
{
	bool quit = false;
	while (!quit)
	{
		// Run image through vision to get observation. 
		Observation obs = vdice(idata->front_img());

		if (obs.dist > 1.0f)
		{
			State desired = calculate(obs, ndata->state);
			atmega::write(desired);
		}
		else
		{
			// std::clog << "Completed first dice." << std::endl;
			quit = true;
		}

		ndata->lock();
		ndata->observation = obs;
		ndata->unlock();
	}
}
