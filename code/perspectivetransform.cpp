#include <iostream>
#include <cmath> // fmax
#include <fstream> // ifstream
#include <cassert> // assert
#include "Color.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;

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

  switch(c) {
    case 'r':
      color.max_s_v = 255; // max saturation and value    
      color.min_s_v = 50;
      color.lower_thresh_lower_val = 0;
      color.lower_thresh_upper_val = 10;
      color.upper_thresh_lower_val = 160;
      color.upper_thresh_upper_val = 179;
      break;
    case 'b':
      color.max_s_v = 255; // max saturation and value    
      color.min_s_v = 50;
      color.lower_thresh_lower_val = 180/2;
      color.lower_thresh_upper_val = 239/2; 
      color.upper_thresh_lower_val = 240/2;
      color.upper_thresh_upper_val = 245/2;
      break;
    case 'g':
      color.max_s_v = 255; // max saturation and value    
      color.min_s_v = 75;
      color.lower_thresh_lower_val = 90/2;
      color.lower_thresh_upper_val = 120/2;
      color.upper_thresh_lower_val = 121/2;
      color.upper_thresh_upper_val = 145/2;
      break;
    case 'y':
      color.max_s_v = 255; // max saturation and value    
      color.min_s_v = 110;
      color.lower_thresh_lower_val = 60/2;
      color.lower_thresh_upper_val = 60/2;
      color.upper_thresh_lower_val = 61/2;
      color.upper_thresh_upper_val = 70/2;
      break;
  }

  cv::inRange(hsv, 
      cv::Scalar(color.lower_thresh_lower_val, color.min_s_v, color.min_s_v), 
      cv::Scalar(color.lower_thresh_upper_val, color.max_s_v, color.max_s_v), 
      color.lower_range);

  cv::inRange(hsv, 
      cv::Scalar(color.upper_thresh_lower_val, color.min_s_v, color.min_s_v), 
      cv::Scalar(color.upper_thresh_upper_val, color.max_s_v, color.max_s_v), 
      color.upper_range);
}

// TODO: take out debugging statements
void checkNumCirclesDetected(const std::vector<cv::Vec3f>& red_circles, 
    const std::vector<cv::Vec3f>& blue_circles, 
    const std::vector<cv::Vec3f>& green_circles,
    const std::vector<cv::Vec3f>& yellow_circles) {

  int num_circles_detected = 0;

  red_circles.num_circles = red_circles.size();
  blue_circles.num_circles = blue_circles.size();
  green_circles.num_circles = green_circles.size();
  yellow_circles.num_circles = yellow_circles.size();

  std::cout << "num red circles = " << red_circles.size() << std::endl;
  std::cout << "num blue circles = " << blue_circles.size() << std::endl;
  std::cout << "num green circles = " << green_circles.size() << std::endl;
  std::cout << "num yellow circles = " << yellow_circles.size() << std::endl;






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
      << "Number of circles detected = " << num_circles_detected << '\n'
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

void hsvUpdate(const std::vector<cv::Vec3f>& red_circles, 
    const std::vector<cv::Vec3f>& blue_circles, 
    const std::vector<cv::Vec3f>& green_circles,
    const std::vector<cv::Vec3f>& yellow_circles) {

  if(red_circles.size() < 1)

  if(blue_circles.size() < 1) 

  if(green_circles.size() < )


  
}

void perspectiveTransform(std::string str) {
  // (0) read file in
  cv::Mat src = readInputImage(str);
  cv::Mat src_detected_circles = src.clone(); // image containing outline around detected circles
  cv::Mat hsv; // image in HSV color space
  cv::Mat dst; // final, "squared up" image

  // (1) convert rgb space to hsv
  cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV); 

  // (2) establish two threshold ranges for what is considered red/blue/green/yellow
  Color red, blue, green, yellow;




  std::cout << "Testing various HSV values until we detect exactly one circle for each color...\n";  



  // loop exits if and only if one of each circle is detected
  while(red.num_circles != 1 || blue.num_circles != 1 || green.num_circles != 1 || yellow.num_circles != 1) {
    findAreasWithColor(red, 'r', hsv);
    findAreasWithColor(blue, 'b', hsv);
    findAreasWithColor(green, 'g', hsv);
    findAreasWithColor(yellow, 'y', hsv);

    // (3) for each color, combine the two detected thresholds into one image,
    //     to be stored in color.hue_image
    cv::addWeighted(red.lower_range, 1.0, red.upper_range, 1.0, 0.0, red.hue_image);
    cv::addWeighted(blue.lower_range, 1.0, blue.upper_range, 1.0, 0.0, blue.hue_image);
    cv::addWeighted(green.lower_range, 1.0, green.upper_range, 1.0, 0.0, green.hue_image);
    cv::addWeighted(yellow.lower_range, 1.0, yellow.upper_range, 1.0, 0.0, yellow.hue_image);

    // (4) for each color, apply Gaussian blur as a denoising step
    cv::GaussianBlur(red.hue_image, red.hue_image, cv::Size(9,9), 2, 2);
    cv::GaussianBlur(blue.hue_image, blue.hue_image, cv::Size(9,9), 2, 2);
    cv::GaussianBlur(green.hue_image, green.hue_image, cv::Size(9,9), 2, 2);
    cv::GaussianBlur(yellow.hue_image, yellow.hue_image, cv::Size(9,9), 2, 2);

    // (5) detect circles in red.hue_image, blue.hue_image, etc. and store circles in red_circles.
    //     circles are encoded with 3 floats: an (x,y) coordinate and a radius.
    //     Note: HoughCircles' params 6, 7 may need to be tuned.
    cv::HoughCircles(red.hue_image, red.circles, CV_HOUGH_GRADIENT, 1, red.hue_image.rows/8, 100, 20, 0, 0);
    cv::HoughCircles(blue.hue_image, blue.circles, CV_HOUGH_GRADIENT, 1, blue.hue_image.rows/8, 100, 20, 0, 0);
    cv::HoughCircles(green.hue_image, green.circles, CV_HOUGH_GRADIENT, 1, green.hue_image.rows/8, 100, 20, 0, 0);
    cv::HoughCircles(yellow.hue_image, yellow.circles, CV_HOUGH_GRADIENT, 1, yellow.hue_image.rows/8, 100, 20, 0, 0);

    cv::resize(yellow.hue_image, yellow.hue_image, cv::Size(), 0.25, 0.25, cv::INTER_LINEAR); // resize the image to 0.25x
    cv::resize(red.hue_image, red.hue_image, cv::Size(), 0.25, 0.25, cv::INTER_LINEAR); // resize the image to 0.25x
    cv::resize(blue.hue_image, blue.hue_image, cv::Size(), 0.25, 0.25, cv::INTER_LINEAR); // resize the image to 0.25x
    cv::resize(green.hue_image, green.hue_image, cv::Size(), 0.25, 0.25, cv::INTER_LINEAR); // resize the image to 0.25x

    cv::imshow("yellow", yellow.hue_image);
    cv::imwrite("yellow.jpg", yellow.hue_image);
    cv::waitKey(0);
    cv::imshow("red", red.hue_image);
    cv::imwrite("red.jpg", red.hue_image);
    cv::waitKey(0);
    cv::imshow("blue", blue.hue_image);
    cv::imwrite("blue.jpg", blue.hue_image);
    cv::waitKey(0);
    cv::imshow("green", green.hue_image);
    cv::imwrite("green.jpg", green.hue_image);
    cv::waitKey(0);

    cv::Mat temp = src_detected_circles;

    // (6) draw detected circles on our image
    identifyCirclesOnImage(red.circles, src_detected_circles);
    identifyCirclesOnImage(blue.circles, src_detected_circles);
    identifyCirclesOnImage(green.circles, src_detected_circles);
    identifyCirclesOnImage(yellow.circles, src_detected_circles);

    cv::resize(src_detected_circles, src_detected_circles, cv::Size(), 0.25, 0.25, cv::INTER_LINEAR); // resize the image to 0.25x
    cv::imshow("identified circles", src_detected_circles);
    cv::waitKey(0);

    // (7) error check, make sure we've detected a non-zero amount of corners
    checkNumCirclesDetected(red.circles, blue.circles, green.circles, yellow.circles);
    hsvUpdate(red.circles, blue.circles, green.circles, yellow.circles);
  }

  // (8) the perspective transformation
  std::vector<cv::Point2f> quadrilateral_pts; // points from our original image
  populateQuadrilateralCorners(red.circles, blue.circles, green.circles, yellow.circles, quadrilateral_pts);

  // square_pts contains points of our transformed image, 
  // with corners (0,0), (0, maxHeight), (maxWidth, 0), (maxWidth, maxHeight)
  std::vector<cv::Point2f> square_pts;
  int warpedEdgeSize = getEdgeSizeOfWarpedImage(red.circles, blue.circles, green.circles, yellow.circles);
  populateSquareCorners(red.circles, blue.circles, green.circles, yellow.circles, warpedEdgeSize, square_pts);

  // M, our transformation matrix required by warpPerspective
  cv::Mat M = getPerspectiveTransform(quadrilateral_pts, square_pts); 

  dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
  cv::warpPerspective(src, dst, M, src.size());

  // (9) crop image to include only the board
  cv::Rect myROI(0, 0, warpedEdgeSize, warpedEdgeSize);
  dst = dst(myROI);

  //	 cv::resize(src_detected_circles, src_detected_circles, cv::Size(), 0.25, 0.25, cv::INTER_LINEAR); // resize the image to 0.25x
  //	 cv::resize(dst, dst, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR); // resize the image to 0.25x
  cv::imshow("asdf", dst);
  cv::waitKey(0);
  cv::imshow("detected circles", src_detected_circles);
  cv::waitKey(0);

  cv::imwrite("move.jpg", dst);
}
