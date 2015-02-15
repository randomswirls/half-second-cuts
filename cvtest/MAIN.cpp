#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;


CvCapture * capture = 0;
IplImage * img = 0; // image for getting the frame from the video
int nFrames;


void getNextFrame()
{
	if(!cvGrabFrame(capture))
		return;          // capture a frame
	img = cvRetrieveFrame(capture);  // retrieve the captured frame
	return;
}

void init()
{
	string inputName = "intest_ferro.avi";
	capture = cvCaptureFromAVI(inputName.c_str());

	cvQueryFrame(capture); // this call is necessary to get correct capture properties
	int frameH    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT); // get height
	int frameW    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH); // get width
	int fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS); // get fps for output
	nFrames = (int) cvGetCaptureProperty(capture,  CV_CAP_PROP_FRAME_COUNT); //CAREFUL, might not work
		//bgimg = cvLoadImage(bgName.c_str(),CV_LOAD_IMAGE_COLOR);
}

void viewVideo()
{
	init();

	for(int i=0;i<nFrames;i++)
	{
		// Read next frame and convert
		getNextFrame();

		// Do all the fun stuff
		//processFrame();

		// Wait for a keystroke so you can view the frame
		cvShowImage("currentFrame", img);
		cvWaitKey(0);

	}

	cvDestroyWindow("currentFrame");
	cvReleaseCapture(&capture);



}
void testLOL()
{
	Mat image;
    image = imread("lol.png", IMREAD_COLOR); // Read the file

    if(! image.data ) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl ;
        exit( -1);
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    imshow( "Display window", image ); // Show our image inside it.

    waitKey(0); // Wait for a keystroke in the window
}

int main( int argc, char** argv )
{
    /*if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }*/
	viewVideo();
   
    return 0;
}