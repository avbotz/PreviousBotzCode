#ifndef MODELING_MCL_HPP 
#define MODELING_MCL_HPP 

#include <iostream>

#include <armadillo>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "interface/ndata.hpp"

static const double DX = 0.1;
static const double DY = 0.1;
static const int X = 100;
static const int Y = 100;
static const int XMAX = 1000;
static const int YMAX = 1000;

static const double DEV_X = 3.0;
static const double DEV_Y = 3.0;

struct MCL 
{
	NData *data;
	arma::Mat<double> grid;
	arma::Mat<double> ndist;

	double heading;
	int SX, SY;
	int WX, WY;

	void init(NData *data);
	void m_update();
	void s_update();
	void resample();
};

#endif
