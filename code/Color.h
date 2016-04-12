#ifndef COLOR_H
#define COLOR_H

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

struct Color {
  int num_circles;                   // number of circles detected by algorithm

  int max_s_v; // max Saturation and Value (S, V in HSV)
  int min_s_v;

  int lower_thresh_lower_val;
  int lower_thresh_upper_val;
  int upper_thresh_lower_val;
  int upper_thresh_upper_val;

  cv::Mat lower_range;              // areas of original image has pixels in the lower hue range
  cv::Mat upper_range;              // ------------------- " ------------------  upper hue range
  cv::Mat hue_image;                // aggregate of lower_range and upper_range
  std::vector<cv::Vec3f> circles;   // contains all detected circles of color Color
};

Color::Color() {
  num_circles = 0;
  max_s_v = 255;
  min_s_v = 50;
}

#endif
