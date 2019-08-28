#include <iostream>
#include <cmath>
#include <thread>

#include "common/log.hpp"
#include "common/config.hpp"
#include "control/connect.hpp"


/*
 * TODO Finish the mechanical interfacing, might need separate functions here.
 */
namespace atmega 
{    
	FILE* in = fopen(PORT, "r+");
	FILE* out = fopen(PORT, "w+");

	void write(std::string command)
	{
		// Don't forget to add a \n to a command sent to this function.
		fprintf(out, command.c_str());
		fflush(out);
	}

	void write(const State &state) 
	{
		// Write to pipe(out) with state data and flush.
		fprintf(out, "s %f %f %f %f %f %f\n", state.x, state.y, state.z, state.yaw, state.pitch, state.roll);
		fflush(out);
	}

	bool read_alive()
	{
		// Read kill state from control and return.
		if (CONNECT_MODE == ConnectMode::SUB) 
		{
			// Request for kill. 
			write("a\n");
			
			// Read from in pipe.
			int kill;
			fscanf(in, "%i", &kill);

			return kill != 0;			
		}

		// Otherwise, return that the sub is alive so dependent programs keep running.
		return true;
	}
	
	State read_state()
	{	
		// Read state from control and return.
		if (CONNECT_MODE == ConnectMode::SUB) 
		{
			// Request for state.
			write("c\n");
			
			// Read from in pipe, using %f for floats.
			float x, y, z, yaw, pitch, roll;
			fscanf(in, "%f %f %f %f %f %f", &x, &y, &z, &yaw, &pitch, &roll);

			State state(x, y, z, yaw, pitch, roll);
			return state;
		}

		// Read state from simulator and return.
		if (CONNECT_MODE == ConnectMode::SIM)
		{
			/*
			 * Ignore, sim state is received from server.cpp.
			 */
		}
		
		// Send back an empty state.
		if (CONNECT_MODE == ConnectMode::MOCK)
		{
			return State(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		}

		return State(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}
	
	/*
	 * Most of the command functions play a more specific role than the above functions. 
	 */
	namespace cmd 
	{
		void kill()
		{
			std::clog << "Killing submarine!" << std::endl;
			aqualog::text("Killing submarine!");
			set_power(0.0);
		}

		void unkill()
		{
			std::clog << "Unkilling submarine!" << std::endl;
			aqualog::text("Unkilling submarine!");
		}

		void set_power(float val)
		{
			std::string cmd = "p " + std::to_string(val) + "\n";
			write(cmd);
		}
		
		void set_state(const State &state) 
		{
			write(state);
		}
	}
}
