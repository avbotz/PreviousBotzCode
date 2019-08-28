#include "vision/filters.hpp"


cv::Mat equalize_hist(bool b, bool g, bool r, const cv::Mat &input)
{
	std::vector<cv::Mat> channels;
	cv::split(input, channels);

	if (b)
		cv::equalizeHist(channels[0], channels[0]);
	if (g)
		cv::equalizeHist(channels[1], channels[1]);
	if (r)
		cv::equalizeHist(channels[2], channels[2]);

	cv::Mat corrected;
	cv::merge(channels, corrected);

	return corrected;
}

cv::Mat illumination(const cv::Mat &input)
{
	// Convert to diff colorspace and split.
	cv::Mat img;
	std::vector<cv::Mat> channels;
	cv::cvtColor(input, img, cv::COLOR_BGR2Lab);
	cv::split(img, channels);

	// Use CLAHE.
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	cv::Mat temp0, temp1, temp2;
	clahe->setClipLimit(2);
	clahe->apply(channels[0], temp0);
	clahe->apply(channels[1], temp1);
	clahe->apply(channels[2], temp2);
	temp0.copyTo(channels[0]);
	temp1.copyTo(channels[1]);
	temp2.copyTo(channels[2]);
	cv::merge(channels, img);

	// Convert back to BGR.
	cv::Mat dst;
	cv::cvtColor(img, dst, cv::COLOR_Lab2BGR);

	return dst;
}

cv::Mat homomorphic(const cv::Mat &src)
{
	std::vector<cv::Mat> hlsimg;
	cv::Mat tmphls;
	cv::cvtColor(src, tmphls, cv::COLOR_BGR2HLS);
	cv::split(tmphls, hlsimg);
	cv::Mat img = hlsimg[0];

	// apply FFT
	cv::Mat fftimg;
	fft(img, fftimg);

	// apply Butterworth HPS
	cv::Mat filter = butterworth(fftimg, 10, 4, 100, 30);
	cv::Mat bimg;
	cv::Mat bchannels[] = {cv::Mat_<float>(filter), cv::Mat::zeros(filter.size(), CV_32F)};
	cv::merge(bchannels, 2, bimg);
	cv::mulSpectrums(fftimg, bimg, fftimg, 0);

	// apply inverse FFT
	cv::Mat ifftimg;
	cv::idft(fftimg, ifftimg, 32); // originally CV_HAL_DFT_REAL_OUTPUT but that didn't work, so hardcoded it
	

	cv::Mat expimg;
	cv::exp(ifftimg, expimg);

	cv::Mat final;
	hlsimg[0] = cv::Mat(expimg, cv::Rect(0, 0, src.cols, src.rows));
	hlsimg[0].convertTo(hlsimg[0], CV_8U);

	merge(&hlsimg[0], 3, img);
	cv::cvtColor(img, final, cv::COLOR_HLS2BGR);
	return final;
}

void fft(const cv::Mat &src, cv::Mat &dst)
{
	// convert to a 32F mat and take log
	cv::Mat logimg;
	src.convertTo(logimg, CV_32F);
	cv::log(logimg+1, logimg);

	// resize to optimal fft size
	cv::Mat padded;
	int m = cv::getOptimalDFTSize(src.rows);
	int n = cv::getOptimalDFTSize(src.cols);
	cv::copyMakeBorder(logimg, padded, 0, m-logimg.rows, 0, n-logimg.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	// add imaginary column to mat and apply fft
	cv::Mat plane[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
	cv::Mat imgComplex;
	cv::merge(plane, 2, imgComplex);
	cv::dft(imgComplex, dst);
}

cv::Mat butterworth(const cv::Mat &img, int d0, int n, int high, int low)
{
	cv::Mat single(img.rows, img.cols, CV_32F);
	int cx = img.rows / 2;
	int cy = img.cols / 2;
	float upper = high * 0.01;
	float lower = low * 0.01;

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			float radius = sqrt(pow(i - cx, 2) + pow(j - cy, 2));
			single.at<float>(i, j) = ((upper - lower) * (1 / pow(d0 / radius, 2 * n))) + lower;
		}
	}
	return single;
}
