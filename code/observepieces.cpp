#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;
static const Size finalSize(1024, 860);
struct compositeCircle{
  Vec3i circle;
  int numCombines;
};
static void help()
{
  cout << "\nThis program demonstrates circle finding with the Hough transform.\n"
    "Usage:\n"
    "./houghcircles <image_name>, Default is ../data/board.jpg\n" << endl;
}

int const threshold_value = 100;

int main(int argc, char** argv)
{
  cv::CommandLineParser parser(argc, argv,
      "{help h ||}{@image|../data/board.jpg|}"
      );
  if (parser.has("help"))
  {
    help();
    return 0;
  }
  string filename = parser.get<string>("@image");
  if (filename.empty())
  {
    help();
    cout << "no image_name provided" << endl;
    return -1;
  }
  Mat3b img = imread(filename, 1);
  resize(img, img, finalSize);
  Mat3b blurred; 

  cout << "showing image" << endl; 
  imshow("image", img);
  waitKey(0);
  Mat origImg; 
  cvtColor(img, origImg, CV_RGB2GRAY,1);

  Mat cimg;
  vector <compositeCircle> known_circles; 
  for (int i = 1; i < 13; i+=2){
    medianBlur(origImg, cimg, i);
    vector<Vec3f> circles;
    HoughCircles(cimg, circles, HOUGH_GRADIENT, 1, 10,
        100, 30, 10, 30 // change the last two parameters
        // (min_radius & max_radius) to detect larger circles
        );

    for(size_t i = 0; i< circles.size(); ++i){
      bool merged = false;
      Vec3i curCircle = circles[i]; 
      for(size_t j = 0; j < known_circles.size(); ++j){
        Vec3i knownCircle = known_circles[j].circle;
        float centerDist = sqrt(pow(curCircle[0] - knownCircle[0],2) +
            pow(curCircle[1] - knownCircle[1],2));  
        if(centerDist < curCircle[2] || centerDist < knownCircle[2]){
          //the circles are the same, combine them
          int nc = known_circles[j].numCombines;
          knownCircle[0] = (nc*knownCircle[0]+curCircle[0])/(nc+1.0);
          knownCircle[1] = (nc*knownCircle[1]+curCircle[1])/(nc+1.0);
          knownCircle[2] = (nc*knownCircle[2]+curCircle[2])/(nc+1.0);
          known_circles[j].numCombines = nc + 1;
          known_circles[j].circle = knownCircle;
          merged = true; 
          break; 
        }
        
      }
      if(!merged) {  
        compositeCircle newCircle; 
        newCircle.circle = curCircle;
        newCircle.numCombines = 1;
        known_circles.push_back(newCircle);
      }
    }
  }
  for( size_t i = 0; i < known_circles.size(); i++ )
  {
    Vec3i c = known_circles[i].circle;
    circle( cimg, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
    circle( cimg, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
  if(image.at<unsigned char>(c[0], c[1]) > threshold_value)
            {
                known_circles[i].color = white;
            }
            else {
                known_circles[i].color = black;
            }
  
  }
  namedWindow( "imOutput", WINDOW_AUTOSIZE ); // Create a window for display.

  imshow("imOutput", cimg);
  cout << "num circles is: " << known_circles.size() << endl; 
  waitKey(0);
} 
