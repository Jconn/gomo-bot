#ifndef COLOR_H
#define COLOR_H

struct Color {
  cv::Mat lower_range;              // areas of original image has pixels in the lower hue range
  cv::Mat upper_range;              // ------------------- " ------------------  upper hue range
  cv::Mat hue_image;                // aggregate of lower_range and upper_range
  std::vector<cv::Vec3f> circles;   // contains all detected circles of color Color
};

#endif
