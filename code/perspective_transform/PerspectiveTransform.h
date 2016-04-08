#ifndef DISPLAYIMAGE_H
#define DISPLAYIMAGE_H
#include <cmath> // fmax
#include <fstream> // ifstream
#include <cassert> // assert
#include "Color.h"

cv::Mat readInputImage(std::string str);

// third argument must be hsv-valued image
extern void findAreasWithColor(Color& color, char c, const cv::Mat& hsv);

// TODO: take out debugging statements
extern void checkNumCirclesDetected(const std::vector<cv::Vec3f>& red_circles, 
    const std::vector<cv::Vec3f>& blue_circles, 
    const std::vector<cv::Vec3f>& green_circles,
    const std::vector<cv::Vec3f>& yellow_circles); 

extern void identifyCirclesOnImage(const std::vector<cv::Vec3f>& circle, cv::Mat im);

// take in our peculiarly-typed vector of detected circles, output the distance between the two corner markers
extern double euclideanDistance(const std::vector<cv::Vec3f>& circle1, const std::vector<cv::Vec3f>& circle2);

// format the data from origin to destination required by the function which performs our transformation
extern void populateQuadrilateralCorners(const std::vector<cv::Vec3f>& red_circle, 
    const std::vector<cv::Vec3f>& blue_circle,
    const std::vector<cv::Vec3f>& green_circle,
    const std::vector<cv::Vec3f>& yellow_circle,
    std::vector<cv::Point2f>& destination);

extern void populateSquareCorners(std::vector<cv::Vec3f>& red_circle, 
    std::vector<cv::Vec3f>& blue_circle,
    std::vector<cv::Vec3f>& green_circle,
    std::vector<cv::Vec3f>& yellow_circle, 
    int maxEdge,
    std::vector<cv::Point2f>& destination);

extern int getEdgeSizeOfWarpedImage(std::vector<cv::Vec3f>& red_circle, 
    std::vector<cv::Vec3f>& blue_circle,
    std::vector<cv::Vec3f>& green_circle,
    std::vector<cv::Vec3f>& yellow_circle);

// testing functions start here
extern void printCoord(const std::vector<cv::Vec3f>& c, std::string str);

extern void printNumberOfCirclesDetected(std::vector<cv::Point2f>& d); 

#endif

