#ifndef PERSPECTIVETRANSFORM_H
#define PERSPECTIVETRANSFORM_H

#include <vector>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Color.h"
cv::Mat readInputImage(std::string str);

void findAreasWithColor(Color& color, char c, const cv::Mat& hsv);

void checkNumCirclesDetected(const std::vector<cv::Vec3f>& red_circles,
		const std::vector<cv::Vec3f>& blue_circles,
		const std::vector<cv::Vec3f>& green_circles,
		const std::vector<cv::Vec3f>& yellow_circles);

void identifyCirclesOnImage(const std::vector<cv::Vec3f>& circle, cv::Mat im);

double euclideanDistance(const std::vector<cv::Vec3f>& circle1, const std::vector<cv::Vec3f>& circle2);

void populateQuadrilateralCorners(const std::vector<cv::Vec3f>& red_circle,
		const std::vector<cv::Vec3f>& blue_circle,
		const std::vector<cv::Vec3f>& green_circle,
		const std::vector<cv::Vec3f>& yellow_circle,
		std::vector<cv::Point2f>& destination);

int getEdgeSizeOfWarpedImage(std::vector<cv::Vec3f>& red_circle,
		std::vector<cv::Vec3f>& blue_circle,
		std::vector<cv::Vec3f>& green_circle,
		std::vector<cv::Vec3f>& yellow_circle);

void populateSquareCorners(std::vector<cv::Vec3f>& red_circle,
		std::vector<cv::Vec3f>& blue_circle,
		std::vector<cv::Vec3f>& green_circle,
		std::vector<cv::Vec3f>& yellow_circle,
		int maxEdge,
		std::vector<cv::Point2f>& destination);

void perspectiveTransform(std::string str);

#endif
