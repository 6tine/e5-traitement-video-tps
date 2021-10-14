#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

Mat frame;
int frameWidth;
int frameHeigth;

void CallBackFunc(int event, int x, int y, int flags, void* userdata){
  if(EVENT_MOUSEMOVE == event){
    cout << "Mouse position :(" << x << "," << y << ")" << endl;
  }
  if(EVENT_RBUTTONDOWN == event){
    Vec3b point = frame.at<Vec3b>(y,x);
    uchar B = point[0];
    uchar G = point[1];
    uchar R = point[2];
    cout << "RGB pixel : (" << (int) R << "," << (int) G << "," << (int) B << ")" << endl;
  }
  if(EVENT_LBUTTONDOWN == event){
    Mat HSV;
    Mat RGB = frame(Rect(x,y,1,1));
    cvtColor(RGB, HSV, COLOR_BGR2HSV);
    Vec3b point = HSV.at<Vec3b>(0,0);
    int H = point[0]; //hue
    int S = point[1]; //saturation
    int V = point[2]; //value
    cout << "HSV pixel : (" << H << "," << S << "," << V << ")" << endl;
  }
}

void extractCabbage(){
  namedWindow("cabbage", WINDOW_AUTOSIZE);
  namedWindow("cabbageFiltre", WINDOW_AUTOSIZE);
  Mat hsvMat, filtre, cabbageIm;
  cvtColor(frame, hsvMat, COLOR_BGR2HSV);
  // On récupère notre masque/filtre
  inRange(hsvMat, Scalar(39,36,138), Scalar(58,137,241), filtre);
  copyTo(frame, cabbageIm, filtre);
  imshow("cabbageFiltre", filtre);
  imshow("cabbage", cabbageIm);

}

int main(int argc, char** argv){
      int res(0);
  char* filename = argv[1];
  //const char filename[] = "smarties.jpg";
  //img = imread(filename, IMREAD_COLOR/*IMREAD_GRAYSCALE*/);
  VideoCapture video(filename);
  if(!video.isOpened())
  {
    cout << "Cannot load video!" << endl;
    res = -1;
  }
  else
  {
    char key;
    Mat dest;
    // Get video resolution
    frameWidth = video.get(CAP_PROP_FRAME_WIDTH);
    frameHeigth = video.get(CAP_PROP_FRAME_HEIGHT);
    // Create video writer object
    VideoWriter output("output.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(frameWidth, frameHeigth));
    // Loop through available frames
    while (video.read(frame)) {
      key = waitKey(10);
      if('p' == key){
        //Mettre en pause
        while('p' != waitKey(0)){}
        key = 'a';
      }
      if('s' == key){
        imwrite("new_im.png", dest);
      }
      if('q' == key){
          break;
      }
      // Display the frame
		  imshow("Video_feed", frame);
		  // Write video frame to output
		  output.write(frame);
      setMouseCallback("Video_feed", CallBackFunc, NULL);
      extractCabbage();
	} // end while (video.read(frame))

	// Release video capture and writer
	output.release();
	video.release();

	// Destroy all windows
	destroyAllWindows();
  }
    return res;
}