#include "mission/gate.hpp"
#include "mission/dice.hpp"


Model gate::model(0.0, 0.0, 0.0);
Actions gate::actions({});
Goal gate::goal(gate::model, gate::actions, "Gate", gate::run);

Model dice::model(0, 0.0, -0.75);
Actions dice::actions({});
Goal dice::goal(dice::model, dice::actions, "Dice", dice::run);
