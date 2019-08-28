#ifndef MISSION_CONFIG_HPP
#define MISSION_CONFIG_HPP 

#include <vector>

#include "mission/goal.hpp"
#include "mission/dice.hpp"
#include "mission/gate.hpp"

enum
{
	GATE_GOAL,
	DICE_GOAL
};

static std::vector<Goal> goals = {
	gate::goal,
	dice::goal
};

#endif
