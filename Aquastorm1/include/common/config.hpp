#ifndef COMMON_CONFIG_HPP
#define COMMON_CONFIG_HPP 

// TODO This should be implemented as a Config class that reads the config from
// a .txt file. Right now, we need to do a large recompile for a config change.

/*
 * Decide camera input mode:
 * - MOCK_CAPTURE 	= use mock images located in root directory 
 * - WEBCAM_CAPTURE = use images from webcam
 * - LIVE_CAPTURE 	= use images from sub's camera
 * - SIM_CAPTURE	= use images from simulator
 */
enum class CameraMode { MOCK_CAPTURE, WEBCAM_CAPTURE, LIVE_CAPTURE, SIM_CAPTURE };
const CameraMode CAPTURE_MODE = CameraMode::WEBCAM_CAPTURE;

/*
 * Decide mission mode:
 * - STATIC 		= sub turns in place 
 * - DYNAMIC 		= sub moves without modeling 
 * - STANDARD 		= sub moves with modeling
 * - DEADRECKONING	= sub follows predetermined instructions
 */
enum class MissionMode { STATIC, DYNAMIC, STANDARD, DEADRECKONING };
const MissionMode MISSION_MODE = MissionMode::DYNAMIC;

/*
 * Use "/dev/ttyUSB0" if running live.
 * Otherwise, use "pipes/mock_port".
 */
enum class ConnectMode { SUB, SIM, MOCK };
const ConnectMode CONNECT_MODE = ConnectMode::MOCK;
static const char *PORT = "pipes/mock_port";

/*
 * Sub power (speed), between 0 and 1.
 * Do NOT set to a high power if dry running the motors!
 */
static const float POWER_LEVEL = 0.2;

/*
 * Set logs to be on or off.
 */
static const bool LOG = true;

#endif
