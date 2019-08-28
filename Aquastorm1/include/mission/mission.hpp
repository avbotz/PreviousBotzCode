#ifndef MISSION_HPP 
#define MISSION_HPP 

#include "interface/idata.hpp"
#include "interface/ndata.hpp"
#include "interface/cdata.hpp"

void mission(IData*, NData*, CData*);
void deadreckon(NData*, State);

#endif
