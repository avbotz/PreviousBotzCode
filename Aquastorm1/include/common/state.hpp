#ifndef COMMON_STATE_HPP 
#define COMMON_STATE_HPP 

#include <string>

#include "common/observation.hpp"
#include "common/model.hpp"

struct State 
{
	/*
	 * "X" represents direction forward and backward.
	 * "Y" represents direction left and right.
	 * "Z" represents direction up and down.
	 * Yaw/Pitch/Roll should be self-explanatory.
	 */
	State() {}
	State(float _x, float _y, float _z, float _yaw, float _pitch, float _roll) :
		x(_x), y(_y), z(_z), yaw(_yaw), pitch(_pitch), roll(_roll) {}

	float x, y, z;
	float yaw, pitch, roll;

	void add_observation(const Observation&);
};

inline std::ostream & operator<<(std::ostream &s, const State &state) {
	s << "s " + std::to_string(state.x) + " " + std::to_string(state.y) + " " + std::to_string(state.z) + " " +
		std::to_string(state.yaw) + " " + std::to_string(state.pitch) + " " + std::to_string(state.roll);
	return s;
}

#endif
