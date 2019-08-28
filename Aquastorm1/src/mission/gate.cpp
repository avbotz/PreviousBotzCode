#include <thread>

#include "common/angle.hpp"
#include "common/log.hpp"
#include "mission/gate.hpp"
#include "control/connect.hpp"
#include "vision/filters.hpp"
#include "vision/config.hpp"


void gate::run(IData *idata, NData *ndata, CData *cdata)
{
	// Run image through vision to get observation. 
	Observation obs = vgate(idata->front_img());

	// Setup desired state.
	State desired = ndata->state;

	// Ensure that observation returned a positive.
	if (obs.valid)	
	{
		if (std::fabs(obs.y) < 10.0f)
		{
			// Sub is aligned with target, attempt to go through. 
			desired.x += 1.0f;
			atmega::write(desired);	
		}
		else 
		{
			// Sub is not aligned with target, turn towards it. 
			desired.yaw = angle::add(desired.yaw, obs.y);
			atmega::write(desired);	
		}
	}
	else 
	{
		// Otherwise, run general scan for task. 
		State desired = ndata->state;
		desired.yaw = angle::add(desired.yaw, 45.0f);
		atmega::write(desired);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}

	ndata->lock();
	ndata->observation = obs;
	ndata->unlock();
}
