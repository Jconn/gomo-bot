#include <string>
#include <iostream>
#include <vector>  // std::vector
#include <utility> // std::pair, std::make_pair
#include <cassert>

#include "opencv/modules/imgproc/include/opencv2/imgproc.hpp"
#include "opencv/modules/core/include/opencv2/core.hpp" 
#include "opencv/modules/highgui/include/opencv2/highgui.hpp" // functions for I/O
#include "DisplayImage.h"
#include "Color.h"

//     note: the top-down orientation of corners is as follows:
//        
//         red                 blue
//          *--------------------*
//          |                    |
//          |                    |
//          |                    |
//          |                    |
//          |                    |
//          |                    |
//          |                    |
//          *--------------------*
//        green                 yellow 

int main(int argc, char** argv) {

  // (0) read file in
  cv::Mat src = readInputImage(argv[1]);
  cv::Mat src_detected_circles = src.clone(); // image containing outline around detected circles
  cv::Mat hsv; // image in HSV color space
  cv::Mat dst; // final, "squared up" image

  // (1) convert rgb space to hsv
  cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV); 

  // (2) establish two threshold ranges for what is considered red/blue/green/yellow
  // TODO: tune rgb values to match stickers
  Color red, blue, green, yellow;

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

  // (6) error check, make sure we've detected a non-zero amount of corners
  checkNumCirclesDetected(red.circles, blue.circles, green.circles, yellow.circles);

  // (7) draw detected circles on our image
  identifyCirclesOnImage(red.circles, src_detected_circles);
  identifyCirclesOnImage(blue.circles, src_detected_circles);
  identifyCirclesOnImage(green.circles, src_detected_circles);
  identifyCirclesOnImage(yellow.circles, src_detected_circles);

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

  cv::resize(src_detected_circles, src_detected_circles, cv::Size(), 0.25, 0.25, cv::INTER_LINEAR); // resize the image to 0.25x

  cv::imshow("asdf", dst);
  cv::imshow("detected circles", src_detected_circles);

  cv::imwrite("transform_output.png", dst);
  cv::waitKey(0);
  return 0;
}

