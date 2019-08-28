#include "server/server.hpp"


void server(IData *idata, NData *ndata, CData *cdata)
{
	std::clog << "server" << std::endl;

	while (true)
	{
		// Translate ZMQ message to string.
		zmq::message_t mt;
		cdata->recv->recv(&mt);
		std::string input = std::string(static_cast<char*>(mt.data()), mt.size());

		// Image-related request.
		if (input.at(0) == 'i')
		{
			if (input.at(1) == 'g')
			{
				cv::Mat img = (input.at(2) == 'd') ? idata->down_img() : idata->front_img();
				if (img.data) 
				{
					cdata->message(*cdata->recv, img);
				}
				else 
				{
					std::cerr << "Could not find image!" << std::endl;
					cdata->message(*cdata->recv, "i fail");
				}
			}
			else if (input.at(1) == 's')
			{	
				cdata->message(*cdata->recv, "i ok");

				// Receive image from ZMQ.
				zmq::message_t mt2;
				cdata->recv->recv(&mt2);
				std::string buf_encoded = std::string(static_cast<char*>(mt2.data()), mt2.size());
				std::string decoded;
				cdata->b64.Decode(buf_encoded, &decoded);
				std::vector<uchar> buf (decoded.begin(), decoded.end());
				cv::Mat decoded_mat = cv::imdecode(buf, CV_LOAD_IMAGE_COLOR);

				if (input.at(2) == 'f')
				{
					idata->lock();
					idata->set_front_img(decoded_mat);
					idata->unlock();
					cdata->message(*cdata->recv, "i ok");
				}
				else if (input.at(2) == 'd')
				{
					idata->lock();
					idata->set_down_img(decoded_mat);
					idata->unlock();
					cdata->message(*cdata->recv, "i ok");
				}
			}
		}
		
		// State-related request.
		else if (input.at(0) == 's')
		{
			if (input.at(1) == 'g')
			{
				State temp = (input.at(2) == 'c') ? ndata->state : ndata->desired;
				std::ostringstream out;
				out << temp;
				cdata->message(*cdata->recv, out.str());
			}
			else if (input.at(1) == 's')
			{
				float x, y, depth, yaw, pitch, roll;
				int result = 0;
				if (input.at(2) == 'c')
					result = sscanf(input.c_str(), "ssc %f %f %f %f %f %f", &x, &y, &depth, &yaw, &pitch, &roll);
				else if (input.at(2) == 'd')
					result = sscanf(input.c_str(), "ssd %f %f %f %f %f %f", &x, &y, &depth, &yaw, &pitch, &roll); 
				if (result == 6)
				{
					State temp(x, y, depth, yaw, pitch, roll);
					if (input.at(2) == 'c')
					{
						ndata->lock();
						ndata->state = temp;
						ndata->unlock();
					}
					else if (input.at(2) == 'd')
					{
						ndata->lock();
						ndata->desired = temp;
						ndata->unlock();
					}
					std::string msg = "s ok";
					cdata->message(*cdata->recv, msg);
				}
				else
				{
					std::string msg = "s fail";
					cdata->message(*cdata->recv, msg);
				}
			}
		}

		// Kill-status related request.
		else if (input.at(0) == 'k')
		{
			if (input.at(1) == 'g')
			{
				std::ostringstream msg;
				msg << "k " << ndata->alive ? 1 : 0;
				cdata->message(*cdata->recv, msg.str());
			}
			else if (input.at(1) == 's')
			{
				char kill_idx;
				if (sscanf(input.c_str(), "ks %c", &kill_idx) == 1)
				{
					if (kill_idx == '1')
						atmega::cmd::unkill();
					else
						atmega::cmd::kill();
					std::string msg = "k ok";
					cdata->message(*cdata->recv, msg);
				}
				else
				{
					cdata->message(*cdata->recv, "k fail");
				}
			}
		}
	}
}

