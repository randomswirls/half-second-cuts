#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

string inputName = "intest_ferro"; // inputName.avi, an uncompressed avi
string codec = "MPEG";

CvCapture * capture = 0; // input video stream
VideoWriter writer; // output video stream


IplImage * currentFrame = 0; // image for getting the frame from the video
IplImage * previousFrame = 0; // image for getting the frame from the video
IplImage * differenceFrame = 0; // image for getting the frame from the video

int frameH;
int frameW;
int fps;
int nFrames;

int currentOutput = 0; // just an index to output multiple videos
int frameCount = 0; // current number of frames in output video
int targetLength = 40; // target number of frames in the shot

string outputName()
{
	stringstream ss;
	ss<<inputName<<"_out_"<<currentOutput<<".avi";
	return ss.str();
}

void getFrame()
{
	if(!cvGrabFrame(capture))
		return;          // capture a frame
	currentFrame = cvRetrieveFrame(capture);  // retrieve the captured frame
	return;
}

void initWriter()
{
	//writer = cvCreateVideoWriter(outputName().c_str(),CV_FOURCC(codec[0],codec[1],codec[2],codec[3]), fps,cvSize(frameW,frameH),1);
	writer.open(outputName(),0,fps,cvSize(frameW,frameH),1);
	// codec "-1" creates a popup with available options
	//http://docs.opencv.org/doc/tutorials/highgui/video-write/video-write.html
	//http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html
	//http://www.fourcc.org/codecs.php

}

void destroyWriter()
{
	//cvReleaseVideoWriter(&writer); // make sure you release it, or it wont finish writing the file
	writer.release();
}

void init()
{
	capture = cvCaptureFromAVI((inputName+".avi").c_str());

	currentFrame = cvQueryFrame(capture); // Grab and retrieve first frame

	frameH = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT); // get height
	frameW = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH); // get width
	fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS); // get fps for output
	nFrames = (int) cvGetCaptureProperty(capture,  CV_CAP_PROP_FRAME_COUNT); //CAREFUL, might not work

	previousFrame = cvCreateImage( cvSize( currentFrame->width, currentFrame->height ), currentFrame->depth, currentFrame->nChannels );
	differenceFrame = cvCreateImage( cvSize( currentFrame->width, currentFrame->height ), currentFrame->depth, currentFrame->nChannels );
	
	initWriter();
	writer.write(currentFrame);
	//cvWriteFrame(writer,currentFrame); // write first frame

}

void viewVideo()
{
	init();

	for(int i=1;i<nFrames;i++)
	{
		// Read next frame and convert
		cvCopy(currentFrame,previousFrame,NULL);
		getFrame();
		frameCount++;

		// Do all the fun stuff
		cvAbsDiff(previousFrame,currentFrame,differenceFrame); // First take the difference
		CvScalar frameSum = cvSum(differenceFrame); // sums each pixel per channel

		// add each channel sum and divide by number of pixels
		double differenceValue = (frameSum.val[0]+frameSum.val[1]+frameSum.val[2])/(currentFrame->width*currentFrame->height);
		cout<<differenceValue<<endl;

		//cvShowImage("currentFrame", differenceFrame); // Show a frame
		//cvWaitKey(1); // Wait 1ms

		bool isCut = differenceValue>100; // 100 is a good value in my test video

		if(isCut){
			if(frameCount==targetLength)
				currentOutput++;
			frameCount=0;
			destroyWriter();
			initWriter();	
		}

		if(frameCount<targetLength){
			//<<"write frame: "<<frameCount<<endl;
			writer.write(currentFrame);
			//cvWriteFrame(writer,currentFrame);
		}

	}

	//cvWaitKey(0); // wait at the end unteill you press a key
	//cvDestroyWindow("currentFrame");
	
	
	cvReleaseCapture(&capture);
	destroyWriter();

	cout<<"framecount "<<frameCount<<endl;
	// delete the last output file, unless the last shot is the target frame length
	if(frameCount!=targetLength-1)
		remove( outputName().c_str() );
		//cerr<< "Error deleting file" ;


	//char wait;
	//cin>>wait;


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

	viewVideo();
   
    return 0;
}