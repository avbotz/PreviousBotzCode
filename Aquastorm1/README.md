Aquastorm
=========

The software that powers Marlin, an autonomous submarine built by the AVBotz Robotics Team to compete at RoboSub. Visit us at [www.avbotz.com](http://avbotz.com) to learn more.

Getting Started
---------------

**Dependencies:**

* Armadillo C++ Linear Algebra Library
* OpenCV 2.4.9 or above
* FlyCapture SDK

This software stack is meant to run on some Unix-based OS. Use the commands **make** and **make clean** to build and clean the project. It also has a YouCompleteMe project config file for people familiar with the Vim plugin. 


Usage 
-----

For people running the submarine live, start the program by using the commands **cat > /dev/ttyUSB0** and **./bin/aquastorm** from the root directory of the project, in separate terminals (or separate TMUX panes). Change to config file in **include/common/config.hpp** to your circumstance. If running on the sub, use **/dev/ttyUSB0** for the port and **LIVE_CAPTURE** for the camera. Otherwise, use **pipes/mock_port** for the port and either **WEBCAM_CAPTURE** or **MOCK_CAPTURE** for the camera. Do not forget to close the the pipe and exit tmux after you are finished running the program. 
