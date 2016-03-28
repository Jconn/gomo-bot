#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;
using cv::CLAHE;
static const Size finalSize(640, 480);

int MAX_DIFF_Y = 100;
int MAX_DIFF_X = 20;

struct Coordinate{
  int x;
  int y;

  Coordinate(int x, int y);
  Coordinate();
};

Coordinate::Coordinate(int a, int b){
  x = a;
  y = b;
}

Coordinate::Coordinate(){
  x = -1;
  y = -1;
}

struct spot{
//	MoveType owner;
	Coordinate topLeft;
	Coordinate topRight;
	Coordinate botLeft;
	Coordinate botRight;


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
    return 0;
  }
  
  Mat src, dst, dst2, color_dst, mask,tmp,sharp_im;
  if( argc != 2 || !(src=imread(argv[1], 0)).data)
      return 0;
 
    cvtColor( dst, color_dst, CV_GRAY2BGR );

  //display original image
  //resize(src,tmp,finalSize);
  //imshow("Original Image",tmp);

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
	circle(src, corners[i], 10, Scalar(0.), -1);
  }

  //display detected corners
  //namedWindow("Corners",1);
  //resize(src, src, finalSize);
  //imshow("Corners", src);

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


  vector<double> sum_coord;
  for(size_t i = 0; i<corners_x.size(); i ++){
	sum_coord.push_back(corners_x[i].x + corners_x[i].y);
  }
  
  double min = 10000000; 
  int index = 0;
  for(size_t i = 0; i < corners_x.size(); i ++){
	if(sum_coord[i] < min){
		min = sum_coord[i];
		index = i;
	}
  }

  Mat_<Point2f> grid(19,19, Point2f(0,0)) ;
  //Point2f starting_pt = corners_x[index];
  Point2f prev = corners[index];
  vector<Point2f> final_points;
  Point2f extrap;
  //grid.at<Point2f>(0,0) = prev;
  //final_points.push_back(prev);
  int col = 0;
  int row = 0;
  for(size_t i = 0; i < corners_x.size(); i ++){
cout << "Current: " << corners_x[i] << endl;
	if((corners_x[i].x - prev.x) < MAX_DIFF_X){
		if((corners_x[i].y - prev.y) < MAX_DIFF_Y){
			if(prev.y < corners_x[i].y)
				prev.x = corners_x[i].x;
				prev.y = corners_x[i].y;
			//grid.at<Point2f>(col,row) = prev;
			final_points.push_back(corners_x[i]);
			//row = row + 1;
		}
		else{
			extrap.x = prev.x;
			extrap.y = prev.y + 100;
			//row = row + 1;
			prev.x = extrap.x;
			prev.y = extrap.y;
			//grid.at<Point2f>(col,row) = prev;
			final_points.push_back(prev);
		}
	}
	else{
		prev.x = corners_x[i].x;
		prev.y = corners_x[i].y;
		//col = col + 1;
		//row = 0;
		
  		std::sort(final_points.begin(), final_points.end(), 
			[](const cv::Point2f &a, const cv::Point2f &b){
				return a.y < b.y;
		});
		
		for(size_t i = 0; i < final_points.size(); i++){
			grid.at<Point2f>(col,row) = final_points[i];
			row = row + 1;
		}
		col = col + 1;
		row = 0;
		final_points.clear();
		final_points.push_back(prev);
		//grid.at<Point2f>(col,row) = prev;
	}
cout << "Prev: " << prev << endl;
  }


  for(int i = 0; i < grid.rows; i++){
	for(int j = 0; j<grid.cols; j++){
		cout << "Column " << i << " Row " << j << " " << grid.at<Point2f>(i,j) << endl;
	}

  }



  

/*
  spot board[18][18];

  Point2f starting_pt = corners_x[index];
  //board[1][1].topLeft = starting_pt;
  
//  for( size_t i; i< corners_x.size(); i ++){
	for( size_t j; j<18; j++){
		Point2f tmp = corners_x[i]; 
		board[1][j].topLeft = tmp;
	}
 // }
 */
//  vector<Point2f> grid; 
//  grid = corners; 
  cout << "Min " << min << " at index " << index << endl;
  cout << "starting point: " << corners_x[index] << endl;
/*  for(size_t i = 0; i < corners_x.size(); i++){
	grid[i].x = corners_x[i].x;
	grid[i].y = corners_y[i].y;
  }
  cout << corners << endl;
*/
  waitKey(0);
  return 0;
}




