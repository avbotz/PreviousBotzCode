#ifndef CONTROL_CONNECT_HPP
#define CONTROL_CONNECT_HPP

#include <string>

#include "interface/ndata.hpp"
#include "common/state.hpp"

class NData;

namespace atmega 
{
	extern FILE *in;
	extern FILE *out;

	void write(std::string);
	void write(const State&);
	bool read_alive();
	State read_state();

	/*
	 * Subset of atmega that deals with often used commands.
	 */
	namespace cmd 
	{
		void kill();
		void unkill();
		void set_power(float);
		void set_state(const State&);
	}
}

#endif
