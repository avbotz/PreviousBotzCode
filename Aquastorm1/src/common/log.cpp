#include <iostream>
#include <string>
#include <ctime>
#include <sys/stat.h>

#include "common/log.hpp"


namespace aqualog 
{
	std::string filename; 
    std::vector<std::string> pending;

	// Logs images to "log" directory with the current date as the directory and time as the file name.
	void image(const cv::Mat &img, char ending)
	{
		// Get date and time to form directory and file name
		time_t t = std::time(0);
		struct tm now = *std::localtime(&t);
		char date[80], name[80];
		std::strftime(date, sizeof(date), "%Y_%m_%d", &now);
		std::strftime(name, sizeof(name), "%H_%M_%S", &now);
		std::string loc = "logs/" + std::string(date) + "/" + std::string(name) + "_" + std::string(1, ending) + ".png";

		// Create directory if it doesn't exist.
		mkdir(std::string("logs/" + std::string(date)).c_str(), ACCESSPERMS);

		// Write the images to the correct path, but make sure they exist first. 
		if (!img.data)
			std::cerr << "Could not find logging image data." << std::endl;
		if (!cv::imwrite(loc, img))
			std::cerr << "Could not log image." << std::endl;
	}

    void image(const cv::Mat &img, std::string filename) {
        std::string loc = "logs/" + filename;
        if (!cv::imwrite(loc, img)) {
            std::cerr << "Could not write to debug image" << std::endl;
        }
    }

	// Initializes the text log name with the start time of the run. 
	void init()
	{
		// Get date and time to form directory and file name.
		time_t t = std::time(0);
		struct tm now = *std::localtime(&t);
		char date[80], curr[80];
		std::strftime(date, sizeof(date), "%Y_%m_%d", &now);
		std::strftime(curr, sizeof(curr), "%H_%M_%S", &now);
		aqualog::filename = "logs/text/" + std::string(date) + "/" + std::string(curr) + ".txt";
		aqualog::pending = std::vector<std::string>();
		
		// Create directory if it doesn't exist.
		mkdir(std::string("logs/text/" + std::string(date)).c_str(), ACCESSPERMS);
	}

	// Appends to the log file. 
	void text(std::string log) 
	{
		// pending.push_back(log);
        std::ofstream fout;
		fout.open(aqualog::filename, std::ios::app);
		fout << log << std::endl;
		fout.close();
	}
    
    void info(std::string log) {
        log = "@i " + log;
        aqualog::text(log);
    }

    void debug(std::string log) {
        log = "@d " + log;
        aqualog::text(log);
    }

    void error(std::string log) {
        log = "@e " + log;
        aqualog::text(log);
    }

    void critical(std::string log) {
        log = "@c " + log;
        aqualog::text(log);
    }
}
