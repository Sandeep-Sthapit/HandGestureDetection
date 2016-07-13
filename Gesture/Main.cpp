#include "Detector.h"
#include "VProcessor.h"

void draw(cv::Mat& img, cv::Mat& out) {

	img.copyTo(out);
	cv::circle(out, cv::Point(100, 100), 5, cv::Scalar(255, 0, 0), 2);
}

int main()
{
	// Now using the VideoProcessor class
		// Create instance
	VideoProcessor processor;
	// Open video file
	processor.setInput(0);
	// Declare a window to display the video
	//processor.displayInput("Input Video");
	processor.displayOutput("Output Video");
	// Play the video at the original frame rate
	processor.setDelay(1000. / processor.getFrameRate());
	// Set the frame processor callback function
	processor.setFrameProcessor(getSkin);
	// Start the process
	processor.run();

	cv::waitKey();
}