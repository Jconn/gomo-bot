#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;
static const Size finalSize(640, 480);
struct compositeCircle{
  Vec3i circle;
  int numCombines;
};
static void help()
{
  cout << "\nThis program demonstrates line finding with the Hough transform.\n"
    "Usage:\n"
    "./hough_lines <image_name>, Default is ../data/board.jpg\n" << endl;
}


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
  
  Mat src, dst, color_dst;
  if( argc != 2 || !(src=imread(argv[1], 0)).data)
      return -1;
 
  Canny( src, dst, 50, 200, 3 );
  cvtColor( dst, color_dst, CV_GRAY2BGR );

  vector<Vec4i> lines;
  HoughLinesP( dst, lines, 1, CV_PI/180, 80, 30, 10 );
  for( size_t i = 0; i < lines.size(); i++ )
  {
      line( color_dst, Point(lines[i][0], lines[i][1]),
          Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
  }

  namedWindow( "Source", 1 );
  imshow( "Source", src );

  namedWindow( "Detected Lines", 1 );
  resize(color_dst, color_dst, finalSize);
  imshow( "Detected Lines", color_dst );

  waitKey(0);
  return 0;
}
  
