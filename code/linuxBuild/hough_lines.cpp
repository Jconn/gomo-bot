#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;
static const Size finalSize(640, 480);


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
  
  Mat src, dst, dst2, color_dst, mask,tmp,sharp_im;
  if( argc != 2 || !(src=imread(argv[1], 0)).data)
      return -1;
 
  Canny( src, dst, 100, 100*3, 3);
  cvtColor( dst, color_dst, CV_GRAY2BGR );

  //resize(src,src,finalSize);
  //imshow("Original",src);

  //cvtColor(src, src, CV_BGR2GRAY);
  //blur(src,src,Size(3,3));
  //adaptiveThreshold(src, src, 100, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,11,2);
  //resize(src,src,finalSize);
  //imshow("Threshold",src);
  
/*
  namedWindow("original",1);
  resize(src,tmp,finalSize);
  imshow("original",tmp);

  tmp = 0;

  GaussianBlur(src,tmp,Size(5,5), 5);
  
  addWeighted(src, 1.5, tmp, -0.5,0, src);
  sharp_im = src;
  namedWindow("Sharpened Image",1);
  resize(sharp_im, sharp_im, finalSize);
  imshow("Sharpened Image", sharp_im);
*/

  sharp_im = src; 
  equalizeHist(src, sharp_im);
  

  namedWindow("sharpened image", 1);
  resize(sharp_im,sharp_im,finalSize);
  imshow("sharpened image", sharp_im);

  vector<Point2f>corners;
  goodFeaturesToTrack(src,corners,370, 0.1, 50, mask, 18, 10, 0.1); //qualit = 0.1, min = 50

  for(size_t i = 0; i < corners.size(); i++)
  {
	circle(src, corners[i], 10, Scalar(255.), -1);
  }

  namedWindow("Corners",1);
  resize(src, src, finalSize);
  imshow("Corners", src);



  //namedWindow("Edges",1);
  //resize(dst,dst2,finalSize);
  //imshow("Edges",dst2);

  vector<Vec4i> lines;
  HoughLinesP( dst, lines, 1, CV_PI/180, 50, 30, 10 );
  for( size_t i = 0; i < lines.size(); i++ )
  {
      line( color_dst, Point(lines[i][0], lines[i][1]),
          Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
  }

 // namedWindow( "Source", 1 );
  //resize(src,src,finalSize);
  //imshow( "Source", src );

  //namedWindow( "Detected Lines", 1 );
  //resize(color_dst, color_dst, finalSize);
  //imshow( "Detected Lines", color_dst );

  waitKey(0);
  return 0;
}
  
