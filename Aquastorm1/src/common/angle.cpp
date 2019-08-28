#include <cmath>

#include "common/angle.hpp"
#include "vision/config.hpp"


double angle::difference(double a1, double a2)
{	
	double c1 = std::fabs(a1 - a2);
	double c2 = std::fabs(a1 - a2 + 360);
	double c3 = std::fabs(a1 - a2 - 360);
	if (c1 < c2 && c1 < c3)
		return c1;
	if (c2 < c3)
		return c2;
	else 
		return c3;
}

double angle::add(double a1, double a2)
{
	double a = a1 + a2;
	if (a < 0)
		a += 360; 
	if (a > 360)
		a -= 360;
	return a;
}

double angle::subtract(double a1, double a2)
{	
	double a = a1 - a2;
	if (a < 0)
		a += 360; 
	if (a > 360)
		a -= 360;
	return a;
}

double angle::calibrate(int x, int width)
{
	double mid = width/2;
	double dif = x - mid;
	double yaw = dif/width * FOV;
	return yaw;
}
