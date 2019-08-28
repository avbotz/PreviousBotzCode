#include <cmath>

#include "common/state.hpp"


/*
 * This method should only be used when modeling is INACTIVE!
 * It updates the state based on the observation we give it, if the observation is good.
 */
void State::add_observation(const Observation &obs)
{
	this->yaw += obs.y;
	this->pitch += obs.p;
	this->roll += obs.r;
}
