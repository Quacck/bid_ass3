#include "touchVisualizer.hpp"
using namespace cv;
void TouchVisualizer::draw(cv::Mat & frame, const cv::RotatedRect & position, cv::Mat & output)
{
    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/
	output = frame.clone();
	ellipse(output, position, Scalar(0, 255, 255), 5, 8);
}
