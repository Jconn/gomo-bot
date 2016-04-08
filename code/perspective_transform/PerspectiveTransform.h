#ifndef DISPLAYIMAGE_H
#define DISPLAYIMAGE_H
#include <cmath> // fmax
#include <fstream> // ifstream
#include <cassert> // assert
#include "Color.h"



cv::Mat readInputImage(std::string str) {
  cv::Mat image = cv::imread(str, CV_LOAD_IMAGE_COLOR); 

  // check for invalid input
  if(!image.data) {
    std::cout <<  "Could not open or find the image" << '\n';
    exit(1);
  }
  return image;
}

// third argument must be hsv-valued image
void findAreasWithColor(Color& color, char c, const cv::Mat& hsv) {
  int MAX_S_V = 255; // max saturation and value    
  int MIN_S_V = 100;

  int lower_thresh_lower_val;
  int lower_thresh_upper_val;
  int upper_thresh_lower_val;
  int upper_thresh_upper_val;

  switch(c) {

    case 'r':
      lower_thresh_lower_val = 0;
      lower_thresh_upper_val = 10;
      upper_thresh_lower_val = 160;
      upper_thresh_upper_val = 179;
      break;
    case 'b':
      lower_thresh_lower_val = 180/2;
      lower_thresh_upper_val = 239/2; 
      upper_thresh_lower_val = 240/2;
      upper_thresh_upper_val = 245/2;
      break;
    case 'g':
      lower_thresh_lower_val = 90/2;
      lower_thresh_upper_val = 120/2;
      upper_thresh_lower_val = 121/2;
      upper_thresh_upper_val = 145/2;
      break;
    case 'y':
      lower_thresh_lower_val = 50/2;
      lower_thresh_upper_val = 60/2;
      upper_thresh_lower_val = 61/2;
      upper_thresh_upper_val = 70/2;
      break;
  }

  cv::inRange(hsv, 
      cv::Scalar(lower_thresh_lower_val, MIN_S_V, MIN_S_V), 
      cv::Scalar(lower_thresh_upper_val, MAX_S_V, MAX_S_V), 
      color.lower_range);

  cv::inRange(hsv, 
      cv::Scalar(upper_thresh_lower_val, MIN_S_V, MIN_S_V), 
      cv::Scalar(upper_thresh_upper_val, MAX_S_V, MAX_S_V), 
      color.upper_range);
}

// TODO: take out debugging statements
void checkNumCirclesDetected(const std::vector<cv::Vec3f>& red_circles, 
    const std::vector<cv::Vec3f>& blue_circles, 
    const std::vector<cv::Vec3f>& green_circles,
    const std::vector<cv::Vec3f>& yellow_circles) {

  int num_circles_detected = 0;

  if(red_circles.size() == 1)
    ++num_circles_detected;

  if(blue_circles.size() == 1)
    ++num_circles_detected;

  if(green_circles.size() == 1)
    ++num_circles_detected;
  
  if(yellow_circles.size() == 1)
    ++num_circles_detected;

  if(num_circles_detected != 4) {
    std::cerr << "Detected less than the number of circles possible.\n"
              << "Try changing the hue values or moving to better lighting.\n"
              << "Program exiting.\n";
    exit(1);
  }
}

void identifyCirclesOnImage(const std::vector<cv::Vec3f>& circle, cv::Mat im);

// take in our peculiarly-typed vector of detected circles, output the distance between the two corner markers
double euclideanDistance(const std::vector<cv::Vec3f>& circle1, const std::vector<cv::Vec3f>& circle2);

// format the data from origin to destination required by the function which performs our transformation
void populateQuadrilateralCorners(const std::vector<cv::Vec3f>& red_circle, 
    const std::vector<cv::Vec3f>& blue_circle,
    const std::vector<cv::Vec3f>& green_circle,
    const std::vector<cv::Vec3f>& yellow_circle,
    std::vector<cv::Point2f>& destination);

void populateSquareCorners(std::vector<cv::Vec3f>& red_circle, 
    std::vector<cv::Vec3f>& blue_circle,
    std::vector<cv::Vec3f>& green_circle,
    std::vector<cv::Vec3f>& yellow_circle, 
    int maxEdge,
    std::vector<cv::Point2f>& destination);

int getEdgeSizeOfWarpedImage(std::vector<cv::Vec3f>& red_circle, 
    std::vector<cv::Vec3f>& blue_circle,
    std::vector<cv::Vec3f>& green_circle,
    std::vector<cv::Vec3f>& yellow_circle);

// testing functions start here
void printCoord(const std::vector<cv::Vec3f>& c, std::string str);

void printNumberOfCirclesDetected(std::vector<cv::Point2f>& d); 

#endif

