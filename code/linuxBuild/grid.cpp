#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;
using cv::CLAHE;
static const Size finalSize(640, 480);

int MAX_DIFF_Y = 130;
int MAX_DIFF_X = 20;


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

  //blur and sharpen image 
  medianBlur(src,src, 11); 
  Ptr<CLAHE> clahe = createCLAHE();
  clahe->setClipLimit(2);
  clahe->apply(src,src);
  src.convertTo(src,-1,1.5,-100);

  sharp_im =src;
  //corner detection
  vector<Point2f>corners;
  vector<Point2f>corners_x;
  vector<Point2f>corners_y;
  goodFeaturesToTrack(src,corners,370, 0.1, 50, mask, 18, 10, 0.1); 

  // putting corners into useful format
  
  std::sort(corners.begin(), corners.end(), 
	[](const cv::Point2f &a, const cv::Point2f &b){
		return a.x < b.x;
	});
  
  corners_x = corners;

  //finding the top left corner
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

  Point2f prev(0,0) ;
  prev.x = corners_x[index].x;
  prev.y = corners_x[index].y;

  Point2f prev2(0,0);
  vector<Point2f> final_points;
  Point2f extrap(0,0);
  Point2f last_point(9999,9999);
  corners_x.push_back(last_point);
  int col = 0;
  int row = 0;

  for(size_t i = 0; i < corners_x.size(); i ++){
	if((corners_x[i].x - prev.x) < MAX_DIFF_X){
		final_points.push_back(corners_x[i]);
		prev.x = corners_x[i].x;
		prev.y = corners_x[i].y;
	}
	else{
		prev.x = corners_x[i].x;
		prev.y = corners_x[i].y;
		
  		std::sort(final_points.begin(), final_points.end(), 
			[](const cv::Point2f &a, const cv::Point2f &b){
				return a.y < b.y;
		});
		prev2.x = final_points[0].x;
		prev2.y = final_points[0].y;
		for(size_t j = 0; j < final_points.size(); j++){ 
			if((final_points[j].y - prev2.y) < MAX_DIFF_Y){
				if(prev2.y < final_points[j].y){
					prev2.x = final_points[j].x;
					prev2.y = final_points[j].y;
				}
				grid.at<Point2f>(col,row) = final_points[j];
				row = row + 1;
			}
			else{
				extrap.x = prev2.x;
				extrap.y = prev2.y + 100;
				prev2.x = extrap.x;
				prev2.y = extrap.y;
				j = j -1;
				grid.at<Point2f>(col,row) = prev2;
				row = row + 1;
				final_points.push_back(prev2);
			}
		}

		col = col + 1;
		row = 0;
		final_points.clear();
		if(prev.y < MAX_DIFF_Y){
			final_points.push_back(prev);
		}
		else{
			extrap.x = prev.x;
			extrap.y = prev.y - 100;
			final_points.push_back(extrap);
			final_points.push_back(prev);
		}
	}
  }


  for(int i = 0; i < grid.rows; i++){
	for(int j = 0; j<grid.cols; j++){
		cout << "Column " << i << " Row " << j << " " << grid.at<Point2f>(i,j) << endl;
	}

  }
 
  waitKey(0);
  return 0;
}




