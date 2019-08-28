#ifndef COMMON_OBSERVATION_HPP
#define COMMON_OBSERVATION_HPP 

#include <iostream>

struct Observation
{
	Observation() {}
	Observation(bool _valid) : valid(_valid) {}
	Observation(float _y, float _p, float _r, float _dist, float _conf, int _task) :
		y(_y), p(_p), r(_r), dist(_dist), conf(_conf), task(_task), valid(true) {}

	float y, p, r;
	float dist, conf;
	bool valid;
	int task;
};

inline std::ostream & operator<<(std::ostream &s, const Observation &obs) {
	s << "o " + std::to_string(obs.y) + " " + std::to_string(obs.p) + " " + std::to_string(obs.r) + " " +
		std::to_string(obs.dist) + " " + std::to_string(obs.conf);
	return s;
}

#endif

