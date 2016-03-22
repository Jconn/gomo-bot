#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;
using cv::CLAHE;
static const Size finalSize(640, 480);


static void help()
{
  cout << "\nThis program demonstrates line finding with the Hough transform.\n"
    "Usage:\n"
    "./hough_lines <image_name>, Default is ../data/board.jpg\n" << endl;
}


vector<Point2f> detect_corner(int argc, char** argv)
{
  cv::CommandLineParser parser(argc, argv,
      "{help h ||}{@image|../data/board.jpg|}"
      );
  if (parser.has("help"))
  {
    help();
    exit(EXIT_FAILURE);
  }
  string filename = parser.get<string>("@image");
  if (filename.empty())
  {
    help();
    cout << "no image_name provided" << endl;
    exit(EXIT_FAILURE);
  }
  
  Mat src, dst, dst2, color_dst, mask,tmp,sharp_im;
  if( argc != 2 || !(src=imread(argv[1], 0)).data)
      exit(EXIT_FAILURE);
 
    cvtColor( dst, color_dst, CV_GRAY2BGR );
/*
  //display original image
  resize(src,tmp,finalSize);
  imshow("Original Image",tmp);
*/
  //blur and sharpen image 
  medianBlur(src,src, 11); 
  Ptr<CLAHE> clahe = createCLAHE();
  clahe->setClipLimit(2);
  clahe->apply(src,src);
  src.convertTo(src,-1,1.5,-100);

  sharp_im =src;
/*
  //display sharpened image
  namedWindow("Sharpened Image", 1);
  resize(sharp_im,sharp_im,finalSize);
  imshow("Sharpened Image", sharp_im);
*/
  //corner detection
  vector<Point2f>corners;
  vector<Point2f>corners_x;
  vector<Point2f>corners_y;
  goodFeaturesToTrack(src,corners,370, 0.1, 50, mask, 18, 10, 0.1); 

  for(size_t i = 0; i < corners.size(); i++)
  {
	circle(src, corners[i], 10, Scalar(255.), -1);
  }
/*
  //display detected corners
  namedWindow("Corners",1);
  resize(src, src, finalSize);
  imshow("Corners", src);
*/
  // putting corners into useful format
  //cout<< corners << endl;
  //vector<Point2f>sorted_corners;
  //sorted_corners = corners;
  std::sort(corners.begin(), corners.end(), 
	[](const cv::Point2f &a, const cv::Point2f &b){
		return a.x < b.x;
	});
  corners_x = corners;
  //cout << corners << endl;
	
  std::sort(corners.begin(), corners.end(), 
	[](const cv::Point2f &a, const cv::Point2f &b){
		return a.y < b.y;
	});
  corners_y = corners;
  //cout << corners << " ";
  
  waitKey(0);
  return corners;
};

int main(int argc, char** argv){
	vector<Point2f>corners;
	corners = detect_corner(argc, argv);

  	for(size_t i = 0; i < corners.size(); i++)
  	{	
		cout << corners[i] << endl;
	}
	

	return 0;
}





