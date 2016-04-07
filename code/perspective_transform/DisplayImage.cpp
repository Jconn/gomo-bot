#include <string>
#include <iostream>
#include <vector>  // std::vector
#include <utility> // std::pair, std::make_pair
#include <cassert>

#include "opencv/modules/imgproc/include/opencv2/imgproc.hpp"
#include "opencv/modules/core/include/opencv2/core.hpp" 
#include "opencv/modules/highgui/include/opencv2/highgui.hpp" // functions for I/O
#include "DisplayImage.h"

int main(int argc, char** argv) {
 
  // key data and parameters
  cv::Mat src, src_gray, src_thresh;
  cv::Mat dst, detected_edges;
  int blockSize = 11; // thresholding
  int C = 10;         // thresholding

  // (0) read file in
  src = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);

  if(src.empty()) {
    std::cout << "Invalid file name. Exiting.\n";
    return -1;
  }

  cv::resize(src, src, cv::Size(), 0.25, 0.25, cv::INTER_LINEAR); // resize the image to 0.25x

  // (1) convert src to grayscale image, named src_gray
  cvtColor(src, src_gray, CV_BGR2GRAY); 
  
  // threshold image; kernel size = blockSize, C is a negative constant added to each pixel 
  cv::adaptiveThreshold(src_gray, src_thresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blockSize, C);

  // important data structures 
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  // find all contours in thresholded image
  // - contours (lines) are parameterized by two end points, i.e., two x,y coordinates, i.e., four integers
  // - .clone() makes a deep copy
  cv::findContours(src_thresh.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
 
  // find contour with largest area and its index within contours
  double largest_area = 0;
  int largest_area_idx = 0;
  std::pair<double, int> contour_params = findLargestContour(contours);
  largest_area = contour_params.first;
  largest_area_idx = contour_params.second;

  drawContours(dst, contours, largest_area_idx, cvScalar(255,255,255), CV_FILLED, 8, hierarchy);
  std::vector<std::vector<cv::Point> > contours_poly(1);
  double epsilon = 1;
  cv::approxPolyDP(cv::Mat(contours[largest_area_idx]), contours_poly[0], epsilon, true);
  cv::Rect boundRect = cv::boundingRect(contours[largest_area_idx]);
  cv::Point p1 = contours_poly[0][0];
  cv::Point p2 = contours_poly[0][1];
  cv::Point p3 = contours_poly[0][2];
  cv::Point p4 = contours_poly[0][3];
  cv::line(src, p1, p2, cv::Scalar(0,0,255), 1, CV_AA, 0); 
  cv::line(src, p2, p3, cv::Scalar(0,0,255), 1, CV_AA, 0); 
  cv::line(src, p3, p4, cv::Scalar(0,0,255), 1, CV_AA, 0); 
  cv::line(src, p4, p1, cv::Scalar(0,0,255), 1, CV_AA, 0); 

  cv::rectangle(src,boundRect, cv::Scalar(0,0,255),1,8,0);
  /*
  assert(contours_poly[0].size() == 4);

  std::vector<cv::Point2f> quadrilateral_pts; // the 4 (x,y) coordinates of our original image
  std::vector<cv::Point2f> square_pts;        // --------------- " ------------ transformed image

  populateFourCorners(contours_poly, quadrilateral_pts);
  populateFourCorners(boundRect, square_pts);

  //https://stackoverflow.com/questions/22519545/automatic-perspective-correction-opencv



  cv::Mat M = getPerspectiveTransform(quadrilateral_pts, square_pts); // our transformation matrix for warpPerspective
  cv::Mat dst_hey = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
  cv::warpPerspective(src, dst_hey, M, src.size());
*/

  cv::imshow("src",src);
  //cv::imshow("src_gray",src_gray);
  cv::imshow("src_thresh", src_thresh);
  //cv::imshow("dst", dst);

  cv::waitKey(0);
  return 0;
}
