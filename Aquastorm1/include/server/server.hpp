#ifndef SERVER_HPP
#define SERVER_HPP 

#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

#include <zmq.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "server/base64.h"
#include "control/connect.hpp"
#include "interface/idata.hpp"
#include "interface/ndata.hpp"
#include "interface/cdata.hpp"
#include "common/log.hpp"

void server(IData*, NData*, CData*);

#endif
