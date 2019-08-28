#include <cmath>

#include "mission/config.hpp"
#include "modeling/mcl.hpp"
#include "common/angle.hpp"
#include "common/log.hpp"


void MCL::init(NData *ndata)
{
	data = ndata;
	grid.set_size(XMAX, YMAX);
	grid.fill(0.10);
	heading = data->state.yaw;

	SX = DEV_X/(DX-0.01);
	SY = DEV_Y/(DY-0.01);
	WX = SX*2 + 1;
	WY = SY*2 + 1;
	arma::Mat<double> norm1(WX, 1);
	arma::Mat<double> norm2(WY, 1);
	int ix = 0;
	int iy = 0;
	for (int i = -SX; i <= SX; i++)
	{
		double val = i < 0 ? (double)(i)/DEV_X*DX/16. : (double)(-i)/DEV_X*DX/16.;
		double prb = 1.1 * erfc(-val * M_SQRT1_2);
		norm1(ix) = prb;
		ix += 1;
	}
	for (int i = -SY; i <= SY; i++)
	{
		double val = i < 0 ? (double)(i)/DEV_Y*DY/16. : (double)(-i)/DEV_Y*DY/16.;
		double prb = 1.1 * erfc(-val * M_SQRT1_2);
		norm2(iy) = prb;
		iy += 1;
	}

	ndist = norm1 * arma::trans(norm2);
}

void MCL::m_update()
{
	int dx = (data->state.x-data->previous.x)/DX;
	int dy = (data->state.y-data->previous.y)/DY;	

	if (dx > 0)
	{
		grid = grid.rows(dx, grid.n_rows-1);
		grid.insert_rows(grid.n_rows, dx);
		grid.submat(grid.n_rows-dx, 0, grid.n_rows-1, grid.n_cols-1).fill(0.10);
	}
	else if (dx < 0) 
	{
		grid = grid.rows(0, grid.n_rows+dx-1);
		grid.insert_rows(0, -dx);
		grid.submat(0, 0, -dx, grid.n_cols-1).fill(0.10);
	}

	if (dy > 0)
	{
		grid = grid.cols(dy, grid.n_cols-1);
		grid.insert_cols(grid.n_cols, dy);
		grid.submat(0, grid.n_cols-dy, grid.n_rows-1, grid.n_cols-1).fill(0.10);
	}
	else if (dy < 0)
	{
		grid = grid.cols(0, grid.n_cols+dy-1);
		grid.insert_cols(0, -dy);
		grid.submat(0, 0, grid.n_rows-1, -dy).fill(0.10);
	}
	
	data->lock();
	data->previous = data->state;
	data->unlock();
	heading = data->state.yaw;
}

void MCL::s_update()
{
	if (!data->observation.valid)
		return;

	double obs_head = angle::add(heading, data->observation.y);
	double obs_dist = data->observation.dist;
	double obs_conf = data->observation.conf;
	int obs_task = data->observation.task;

	Model model = goals[obs_task].model;
	double estx = model.x - std::sin(obs_head*M_PI/180.0)*obs_dist;
	double esty = model.y - std::cos(obs_head*M_PI/180.0)*obs_dist;

	int dx = (estx - data->state.x)/DX;
	int dy = (esty - data->state.y)/DY;

	arma::Mat<double> gap = arma::ones(WX, WY);
	arma::Mat<double> sub = grid.submat(XMAX/2-SX+dx, YMAX/2-SY+dy, XMAX/2+SX+dx, YMAX/2+SY+dy);
	gap -= sub;
	gap /= ndist;
	grid.submat(XMAX/2-SX+dx, YMAX/2-SY+dy, XMAX/2+SX+dx, YMAX/2+SY+dy) = arma::ones(WX, WY) - gap;
	
	arma::Mat<double> dbg = arma::trans(grid) * 255;
	cv::Mat img(XMAX, YMAX, CV_64FC1, dbg.memptr());
	cv::Mat dst;
	cv::resize(img, dst, cv::Size(800, 800));
	aqualog::image(dst, 'm');
}

void MCL::resample()
{
	grid *= 0.90;
	for (int r = 0; r < XMAX; r++)
		for (int c = 0; c < YMAX; c++)
			if (grid(r, c) < 0.1)
				grid(r, c) = 0.1;
}
