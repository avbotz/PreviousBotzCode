#ifndef MISSION_DICE_HPP
#define MISSION_DICE_HPP 

#include <opencv2/core/core.hpp>

#include "interface/idata.hpp"
#include "interface/ndata.hpp"
#include "interface/cdata.hpp"
#include "common/model.hpp"
#include "common/state.hpp"
#include "common/observation.hpp"
#include "mission/actions.hpp"
#include "mission/goal.hpp"

namespace dice 
{
	extern Model model;
	extern Actions actions;
	extern Goal goal;
	void run(IData*, NData*, CData*);
}

#endif 
