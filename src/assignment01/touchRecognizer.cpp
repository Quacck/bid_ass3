#include "touchRecognizer.hpp"

#include <algorithm>
#include <iostream>
#include <Windows.h>

using namespace cv;
using namespace std;

void TouchRecognizer::calibrate(const cv::Mat& background)
{
	background.copyTo(m_background);

	int depth = 0;

	Mat backgroundHist;
	
	for (int i = 1; i < m_background.rows; i++) {
		for (int j = 1; j < m_background.cols; j++) {
			if (m_background.at<unsigned short>(i, j) > depth) {

				depth = m_background.at<unsigned short>(i, j);

			}
		}
	}

	backgroundDepth = depth;
	//cout << backgroundDepth;
}

std::vector<cv::RotatedRect> TouchRecognizer::recognize(const cv::Mat& depthFrame) const
{
	std::vector<cv::RotatedRect> positions;

	//remove the background from the current frame
	Mat legFrame = m_background - depthFrame;

	Mat footFrame = Mat(depthFrame.rows, depthFrame.cols, CV_8UC1);


	//isolate the foot from the image and reduce noise
	threshold(legFrame, legFrame, 80, 0, 4);
	GaussianBlur(legFrame, legFrame, Size(5, 5),0,0);
	threshold(legFrame, legFrame, 15, 10000, 0);

	
	//convert to binary image with 8bit color depth

	for (int i = 0; i < legFrame.rows; i++) {
		for (int j = 0; j < legFrame.cols; j++) {
			if (legFrame.at<unsigned short>(i, j) > 10) {
				footFrame.at<char>(i, j) = 255;
			}
			else {
				footFrame.at<char>(i, j) = 0;
			}
		}
	}

	imshow("feet", footFrame);

	//create a vector with the positions of the feet, feet is bigger than 2000 
	vector<vector<Point>> contours;

	findContours(footFrame, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	for (auto &i : contours) {
		if (contourArea(i) > 2000){
			positions.insert(end(positions),fitEllipse(i));
		}

	}
    return positions;
}
