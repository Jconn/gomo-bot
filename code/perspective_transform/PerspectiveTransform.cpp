#include "Color.h"

cv::Mat readInputImage(std::string str) {
  cv::Mat image = cv::imread(str, CV_LOAD_IMAGE_COLOR); 

  // check for invalid input
  if(!image.data) {
    cout <<  "Could not open or find the image" << std::endl ;
    exit(1);
  }
  return image;
}

// third argument must be hsv-valued image
void findAreasWithColor(Color& color, char c, const cv::Mat& hsv) {

  
  
  switch(c) {
    case 'r':
      break;
      case 'b'
        break;
    case 'g':
      break;
    case 'y':
      break;
  }
}

// TODO: take out debugging statements
void checkNumCirclesDetected(const std::vector<cv::Vec3f>& red_circles, 
    const std::vector<cv::Vec3f>& blue_circles, 
    const std::vector<cv::Vec3f>& green_circles,
    const std::vector<cv::Vec3f>& yellow_circles) {

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



