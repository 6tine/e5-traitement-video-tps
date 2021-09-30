/*============================================================================*/
/* File Description                                                           */
/*============================================================================*/
/**
 * @file        load_display.cpp
 * @author      P. FOUBERT
 * @brief       OpenCV project to show how to load and display an image
 */
/*============================================================================*/

/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/
#include <iterator>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

/*============================================================================*/
/* Defines                                                                    */
/*============================================================================*/

/*============================================================================*/
/* Local Constants                                                            */
/*============================================================================*/

/*============================================================================*/
/* Local Types                                                                */
/*============================================================================*/

/*============================================================================*/
/* Global Variables                                                                */
/*============================================================================*/

cv::Mat img;

/*============================================================================*/
/* Static Variables                                                           */
/*============================================================================*/

/*============================================================================*/
/* Static Functions Prototype                                                 */
/*============================================================================*/

/******************************************************************************/
/* Public Functions                                                           */
/******************************************************************************/
void rotateImage(const cv::Mat &i_src, double angle, cv::Mat &dest){
  cv::Point2f pt(static_cast<float>(i_src.cols) / 2,static_cast<float>(i_src.rows) / 2);
  cv::Mat rot = cv::getRotationMatrix2D(pt, angle, 1.0);

  cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), i_src.size(), angle).boundingRect2f();
  // adjust transformation matrix
  rot.at<double>(0,2) += bbox.width/2.0 - i_src.cols/2.0;
  rot.at<double>(1,2) += bbox.height/2.0 - i_src.rows/2.0;

  cv::warpAffine(i_src, dest, rot, bbox.size());

}

void contrasteImage(const cv::Mat &i_src, double step, cv::Mat &dest){
    i_src.convertTo(dest, -1, step, 0);
}

void shineImage(const cv::Mat &i_src, double bright, cv::Mat &dest){
    i_src.convertTo(dest, -1, 1, bright);
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata){
  if(cv::EVENT_MOUSEMOVE == event){
    std::cout << "Mouse position :(" << x << "," << y << ")" << std::endl;
  }
  if(cv::EVENT_RBUTTONDOWN == event){
    cv::Vec3b point = img.at<cv::Vec3b>(y,x);
    uchar B = point[0];
    uchar G = point[1];
    uchar R = point[2];
    std::cout << "RGB pixel : (" << (int) R << "," << (int) G << "," << (int) B << ")" << std::endl;
  }
  if(cv::EVENT_LBUTTONDOWN == event){
    cv::Mat HSV;
    cv::Mat RGB = img(cv::Rect(x,y,1,1));
    cv::cvtColor(RGB, HSV, cv::COLOR_BGR2HSV);
    cv::Vec3b point = HSV.at<cv::Vec3b>(0,0);
    int H = point[0]; //hue
    int S = point[1]; //saturation
    int V = point[2]; //value
    std::cout << "HSV pixel : (" << H << "," << S << "," << V << ")" << std::endl;
  }
}

/*============================================================================*/
/* Function Description                                                       */
/*============================================================================*/
/**
 * @brief         Main
 * @param[in]     argc        : Number of arguments
 * @param[in]     argv        : Arguments list
 * @return        <0 if error, 0 if success, >0 if warning
 */
/*============================================================================*/
int main(int argc, char** argv)
{
  int res(0);
  char* filename = argv[1];
  //const char filename[] = "smarties.jpg";

  img = cv::imread(filename, cv::IMREAD_COLOR/*IMREAD_GRAYSCALE*/);
  if(img.empty())
  {
    std::cout << "Cannot load image!" << std::endl;
    res = -1;
  }
  else
  {
    cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
    cv::Mat dest;
    char key;
    double angle = 0;
    double step = 1, bright = 0;
    while((key = cv::waitKey(0)) != 'q'){
      if('l' == key){
        angle += 15;
      }
      if('r' == key){
        angle -= 15;
      }
      if('s' == key){
        cv::imwrite("new_im.png", dest);
      }
      if('a' == key){
        step += 0.5;
      }
      if('z' == key){
        step -= 0.5;
      }
      if('e' == key){
        bright += 10;
      }
      if('d' == key){
        bright -= 10;
      }
      shineImage(img, bright, dest);
      contrasteImage(dest, step, dest);
      rotateImage(dest, angle, dest);
      cv::setMouseCallback("image", CallBackFunc, NULL);
      cv::imshow("image", dest);
      
    }
  }
    return res;
}
