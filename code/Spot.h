#ifndef SPOT_H
#define SPOT_H 

#include "MoveType.h"
#include "Coordinate.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;
using cv::CLAHE;

struct Spot{
  MoveType owner;
  Point2f loc;  
};
typedef Spot spot;
#endif
