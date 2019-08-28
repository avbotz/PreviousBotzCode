#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "common/log.hpp"
#include "common/config.hpp"
#include "capture/capture.hpp"
#include "capture/camera.hpp"


/*
 * Handles taking images from both the front and down camera.
 */
void capture(IData *data) 
{
	std::clog << "capture" << std::endl;

	// Initialize camera objects.
	Camera front_camera, down_camera;
	bool isfront = front_camera.init(FRONT);
	bool isdown = down_camera.init(DOWN);

	// Create video capture feed (will only be used if needed).
	cv::VideoCapture webcam_camera(0);
	if (CAPTURE_MODE == CameraMode::WEBCAM_CAPTURE && !webcam_camera.isOpened())
	{
		std::cerr << "Could not open camera." << std::endl;
		aqualog::text("Could not open camera.");
	}

	// Check if cameras are available.
	if (!isfront && !isdown) 
	{
		std::cerr << "Both cameras are unavailable." << std::endl;
		aqualog::text("Both cameras are unavailable.");
	}

	// Grab camera images and update data mutex.
	while (true)
	{ 
		// Read images, check if in sim (front = flipped, down = not flipped).
		cv::Mat front, down;

		// Take images from mock source for simulation. 
		if (CAPTURE_MODE == CameraMode::MOCK_CAPTURE)
		{
			front = cv::imread("mock_image.png", 1);
			down = cv::imread("mock_image.png", 1); 
			// aqualog::image(front, 'f');            
			// aqualog::image(down, 'd');
			data->lock();
			data->set_front_img(front);
			data->set_down_img(down);
			data->unlock();
		}

		// Take images from webcam to simulate vision.
		else if (CAPTURE_MODE == CameraMode::WEBCAM_CAPTURE)
		{
			webcam_camera.read(front);
			down = front;
			data->lock();
			data->set_front_img(front);
			data->set_down_img(down);
			data->unlock();
			if (LOG)
			{
				aqualog::image(front, 'f');
				// aqualog::image(down, 'd');
			}
		}

		// Take images from the sub camera if running live.
		else if (CAPTURE_MODE == CameraMode::LIVE_CAPTURE)
		{
			if (isfront) 
			{
				front = front_camera.capture(true);
				if (!front.data)
					std::cerr << "No front camera data." << std::endl;
				if (LOG)
					aqualog::image(front, 'f');            
				data->lock();
				data->set_front_img(front);
				data->unlock();
			}
			if (isdown)
			{
				down = down_camera.capture(false);
				if (LOG)
					aqualog::image(down, 'd');
				data->lock();
				data->set_down_img(down);
				data->unlock();
			}
			else 
			{
				down = cv::imread("mock_image.png", 1);
				if (LOG)
					aqualog::image(down, 'd');
				data->lock();
				data->set_down_img(down);
				data->unlock();
			}
		}

		// Take images from blender.
		else if (CAPTURE_MODE == CameraMode::SIM_CAPTURE)
		{
			/*
			 * Should read from server.cpp, this just handles logging.
			 */
			cv::Mat f = data->front_img();
			cv::Mat d = data->down_img();
			if (f.data && LOG)
				aqualog::image(f, 'f');
			if (d.data && LOG)
				aqualog::image(d, 'd');
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	// Should never reach here, return error.
	std::cerr << "ERROR: Completed capture." << std::endl;
	aqualog::text("ERROR: Completed capture.");
}
