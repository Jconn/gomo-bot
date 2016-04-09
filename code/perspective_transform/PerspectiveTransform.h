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

void identifyCirclesOnImage(const std::vector<cv::Vec3f>& circle, cv::Mat im) {
  for(size_t current_circle = 0; current_circle < circle.size(); ++current_circle) {
    cv::Point center(std::round(circle[current_circle][0]), std::round(circle[current_circle][1]));
    int radius = std::round(circle[current_circle][2]);

    cv::circle(im, center, radius, cv::Scalar(0, 255, 0), 5);
  }
}

// take in our peculiarly-typed vector of detected circles, output the distance between the two corner markers
double euclideanDistance(const std::vector<cv::Vec3f>& circle1, const std::vector<cv::Vec3f>& circle2) {
  cv::Point p1 = cv::Point(circle1[0][0], circle1[0][1]);
  cv::Point p2 = cv::Point(circle2[0][0], circle2[0][1]);

  cv::Point diff = p1 - p2;

  return sqrt(diff.x * diff.x + diff.y * diff.y);
}

// format the data from origin to destination required by the function which performs our transformation
void populateQuadrilateralCorners(const std::vector<cv::Vec3f>& red_circle, 
    const std::vector<cv::Vec3f>& blue_circle,
    const std::vector<cv::Vec3f>& green_circle,
    const std::vector<cv::Vec3f>& yellow_circle,
    std::vector<cv::Point2f>& destination) {

  destination.push_back(cv::Point(red_circle[0][0],red_circle[0][1]));  
  destination.push_back(cv::Point(blue_circle[0][0], blue_circle[0][1]));  
  destination.push_back(cv::Point(green_circle[0][0], green_circle[0][1]));  
  destination.push_back(cv::Point(yellow_circle[0][0], yellow_circle[0][1]));  
}

int getEdgeSizeOfWarpedImage(std::vector<cv::Vec3f>& red_circle, 
    std::vector<cv::Vec3f>& blue_circle,
    std::vector<cv::Vec3f>& green_circle,
    std::vector<cv::Vec3f>& yellow_circle) {
  
  // find distance between vertices of all four circles (no diagonals)
  double d1 = euclideanDistance(red_circle, blue_circle);
  double d2 = euclideanDistance(blue_circle, yellow_circle);
  double d3 = euclideanDistance(yellow_circle, green_circle);
  double d4 = euclideanDistance(green_circle, red_circle);
  
  return fmax(d1, fmax(d2, fmax(d3, d4)));
}

void populateSquareCorners(std::vector<cv::Vec3f>& red_circle, 
    std::vector<cv::Vec3f>& blue_circle,
    std::vector<cv::Vec3f>& green_circle,
    std::vector<cv::Vec3f>& yellow_circle, 
    int maxEdge,
    std::vector<cv::Point2f>& destination) {

  destination.push_back(cv::Point(0, 0));
  destination.push_back(cv::Point(0, maxEdge));
  destination.push_back(cv::Point(maxEdge, 0));
  destination.push_back(cv::Point(maxEdge, maxEdge));
}

#endif

