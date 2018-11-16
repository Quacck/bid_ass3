#include "../framework/kinect.hpp"
#include "touchRecognizer.hpp"
#include "touchVisualizer.hpp"
#include "drawVisualizer.hpp" // only for bonus assignment

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>



using namespace cv;
using namespace std;

int main()
{
    // initialize framework
    Kinect::initialize();
    Kinect::listDevices();

    // set up device
    Kinect kinect;
    if(!kinect.ready())
    {
        cout << "Could not set up Kinect. Exiting." << std::endl;
        return -1;
    }

    // prepare touch pad
	TouchRecognizer touchRecognizer;
	TouchVisualizer touchVisualizer;

    // prepare variables
    Mat depthFrame;
    Mat colorFrame;
	Mat touchFrame;
    auto running = true;
	vector<cv::RotatedRect> positions;


    // calibrate
	kinect.getDepthFrame(depthFrame);
	touchRecognizer.calibrate(depthFrame);
	//touchRecognizer.createHistogram(depthFrame);

    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/

    // prepare windows - this isn't necessary, but it allows to assign useful positions
    namedWindow("color");
    namedWindow("depth");
	namedWindow("touch");

    /*~~~~~~~~~~~~~~~*
     * DEBUG WINDOWS *
     * COULD GO HERE *
     *~~~~~~~~~~~~~~~*/

    // move windows
    const auto xOffset = colorFrame.cols;
    const auto yOffset = colorFrame.rows;
    cv::moveWindow("color", 1 * xOffset, 0 * yOffset);
    cv::moveWindow("depth", 2 * xOffset, 0 * yOffset);

    /*~~~~~~~~~~~~~~~*
     * DEBUG WINDOWS *
     * COULD GO HERE *
     *~~~~~~~~~~~~~~~*/

	Point lastPoint = (-1,-1);
	float length = 0;


    while(running)
    {
        // update frames
        kinect.getDepthFrame(depthFrame);
        kinect.getColorFrame(colorFrame);
		touchFrame = Mat(colorFrame.size(), CV_8UC3);
        // run touch recognizer
        
		positions=touchRecognizer.recognize(depthFrame);

        /*~~~~~~~~~~~*
         * YOUR CODE *
         * GOES HERE *
         *~~~~~~~~~~~*/


		

        // run visualizer - there may be no position
		if (positions.size() != 0) 
		{
			for (auto &i : positions)
			{
				touchVisualizer.draw(touchFrame, i, touchFrame);

			}
		}

        /*~~~~~~~~~~~*
         * YOUR CODE *
         * GOES HERE *
         *~~~~~~~~~~~*/

        // show frames
        auto depthFrameUnscaled = depthFrame.clone();


		depthFrame = 20 * depthFrame;
        cv::imshow("depth", depthFrame);
        cv::imshow("color", colorFrame);
		cv::imshow("touch", touchFrame);

        // check for keypresses
        const auto key = cv::waitKey(10);
        switch (key)
        {
		case 0x1b: // esc - exit
            running = false;
            break;
		case 0x0d: // enter - calibrate again
			kinect.getDepthFrame(depthFrame);
			touchRecognizer.calibrate(depthFrame);
            break;
        default:
            break;
        }
    }
}
