#ifndef MISSION_GOAL_HPP 
#define MISSION_GOAL_HPP 

#include <functional>

#include <opencv2/core/core.hpp>

#include "interface/cdata.hpp"
#include "interface/ndata.hpp"
#include "interface/idata.hpp"
#include "common/state.hpp"
#include "common/model.hpp"
#include "mission/actions.hpp"

/*
 * Goal represents a task to complete.
 */
struct Goal
{
    Model model;
    Actions actions;
    std::string name;
	std::function<void (IData *idata, NData *ndata, CData *cdata)> run;

	/*
    template <typename M, typename N, typename F, typename G, typename... A>
   	Goal(M _model, N _name, F _func, G _goal_actions, A... _args) :
    	model(_model), vision(std::bind(_func, std::placeholders::_1, _args...)), name(_name), actions(_goal_actions) {}
	*/
	Goal(Model m, Actions a, std::string n, auto r) :
		model(m), actions(a), name(n), run(std::bind(r, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)) {}
};

#endif
