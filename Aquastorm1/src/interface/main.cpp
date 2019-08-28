#include <thread>
#include <iostream>
#include <signal.h>

#include "common/log.hpp"
#include "control/connect.hpp"
#include "interface/idata.hpp"
#include "interface/ndata.hpp"
#include "interface/cdata.hpp"
#include "capture/capture.hpp"
#include "control/control.hpp"
#include "mission/mission.hpp"
#include "modeling/modeling.hpp"
#include "server/server.hpp"


/*
 * Handle program exit, need to set power and speed to 0.
 */
void terminate(int e)
{
	std::cout << "\nCaught exit signal! " << e << std::endl;
	atmega::cmd::kill();
	std::cout << "Exiting Aquastorm..." << std::endl;

	exit(0);
}

/* 
 * Main part of the program, handles intra-process communication and assigns each function to a separate thread.
 */
int main() 
{
	// Catch CTRL-C Signal.
	signal(SIGINT, terminate);

	// Start logger.
	aqualog::init();

	// Setup ZMQ to send and receive.
	zmq::context_t context(1);
	zmq::socket_t send(context, ZMQ_REQ);
	zmq::socket_t recv(context, ZMQ_REP);
	send.connect("tcp://localhost:5555");
	recv.bind("tcp://*:5554");

	// Construct data mutexes.
	IData idata;
	NData ndata;
	CData cdata(&send, &recv);

	// Initialize processes on sub.
	auto capture_thread = std::thread(capture, &idata);
	auto control_thread = std::thread(control, &ndata);
	auto mission_thread = std::thread(mission, &idata, &ndata, &cdata);
	auto modeling_thread = std::thread(modeling, &ndata);
	auto server_thread = std::thread(server, &idata, &ndata, &cdata);

	// Wait for completion, which should never happen.
	capture_thread.join();
	control_thread.join();
	mission_thread.join();
	modeling_thread.join();
	server_thread.join();

	std::cerr << "ERROR: Completed interface." << std::endl;
	aqualog::text("ERROR: Completed interface.");
}
