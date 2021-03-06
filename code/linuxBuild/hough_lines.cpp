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
  resize(src,tmp,finalSize);
  imshow("Original Image",tmp);

  //blur and sharpen image
  //to adjust for darkness/glare
  //src.convertTo(src,-1,1,120);
  //final settings: 
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
  namedWindow("Corners",1);
  resize(src, src, finalSize);
  imshow("Corners", src);

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
cout << "Number of corners detected: " << corners_x.size() << endl;
  //Mat_<Point2f> grid(19,19, prev) ;
  Mat grid = (Mat_<Point2f>(19,19,Point2f(0,0))); 
  //Point2f starting_pt = corners_x[index];
  Point2f prev(0,0) ;
  prev.x = corners_x[index].x;
  prev.y = corners_x[index].y;
  Point2f prev2(0,0);
  Point2f prev3(0,0);
  vector<Point2f> final_points;
  Point2f extrap(0,0);
  Point2f last_point(9999,9999);
  corners_x.push_back(last_point);
  //grid.at<Point2f>(0,0) = prev;
  //final_points.push_back(prev);
  int col = 0;
  int row = 0;
//  int x = 0;
//  int y = 0;
  int num = 0; //points to extrapolate at the end of a column
  for(size_t i = 0; i < corners_x.size(); i ++){
	if((corners_x[i].x - prev.x) < MAX_DIFF_X){		
		final_points.push_back(corners_x[i]);
		prev.x = corners_x[i].x;
		prev.y = corners_x[i].y;
//cout << "Pushed Back Point: " << corners_x[i] << endl;
	}
	else{
//cout << "Point not pushed: " << corners_x[i] << endl;
		prev.x = corners_x[i].x;
		prev.y = corners_x[i].y;
		//col = col + 1;
		//row = 0;
		
  		std::sort(final_points.begin(), final_points.end(), 
			[](const cv::Point2f &a, const cv::Point2f &b){
				return a.y < b.y;
		});

cout << "SIZE: " << final_points.size() << endl;
		prev3 = final_points[0]; 
		for(size_t p = 1; p < final_points.size(); p++){ 
			if((final_points[p].y - prev3.y) < 50){
//cout << "point being erased: " << final_points[p] << endl << endl;
				final_points.erase(final_points.begin() + p);
			}
			prev3.x = final_points[p].x;
			prev3.y = final_points[p].y;
			}
cout << "New size: " << final_points.size() << endl;
cout << "Points: " << endl << final_points << endl;
		prev2.x = final_points[0].x;
		prev2.y = final_points[0].y;
//cout << endl << endl << endl;
		for(size_t j = 0; j < final_points.size(); j++){ 
//cout << "Current Point: " << final_points[j] << "   Prev Point: " << prev2 << endl;
			if((final_points[j].y - prev2.y) < MAX_DIFF_Y){

/*
			if(corners_x[j].y > MAX_DIFF_Y && j == 0){
				y = (int)corners_x[j].y;
				x = y /  MAX_DIFF_Y;
cout << "x: " << x << endl;
				extrap.x = corners_x[j].x;
				extrap.y = corners_x[j].y - 100;
				grid.at<Point2f>(col,row) = extrap;
				row = row + 1;	
cout << "Point extrapolated: " << extrap << endl;
				}
*/
				if(prev2.y < final_points[j].y){
					prev2.x = final_points[j].x;
					prev2.y = final_points[j].y;
				}
//cout << "Point is in the right order: " << final_points[j] << endl;
				if( j == 0 && final_points[0].y > MAX_DIFF_Y){
					extrap.x = final_points[0].x;
					extrap.y = final_points[0].y - 100;
					grid.at<Point2f>(col,row) = extrap;
					row = row + 1;
				}
				grid.at<Point2f>(col,row) = final_points[j];
				//final_points.push_back(corners_x[i]);
				row = row + 1;
			}
			else{
				extrap.x = prev2.x;
				extrap.y = prev2.y + 100;
				//row = row + 1;
				prev2.x = extrap.x;
				prev2.y = extrap.y;
				j = j -1;
				grid.at<Point2f>(col,row) = prev2;
				row = row + 1;
//cout << "Extrapolated point: " << extrap << endl;
			//	final_points.push_back(prev2);
			}
		/*
			for(size_t i = 0; i < final_points.size(); i++){
				grid.at<Point2f>(col,row) = final_points[i];
				row = row + 1;
			}*/

///////////////////////////// PUT BELOW IN FINAL CODE! ///////////////////////////////////
cout << "Row = " << row << endl;
			if(row < 19 && j == (final_points.size()-1) ){
				num = 19 - row ;
cout << "Num: " << num << endl;
				for(int k = num; k > 0; --k){
					extrap.x = prev2.x;
					extrap.y = prev2.y + 100;
cout << "Extrapolated Point: " << extrap <<  endl;
					grid.at<Point2f>(col,row) = extrap;
					prev2.x = extrap.x;
					prev2.y = extrap.y;
					row = row + 1;
				}

  cout << " number of references is " << grid.u->refcount << endl;

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
		//grid.at<Point2f>(col,row) = prev;
	}
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
  //  waitKey(0);
  cout << " number of references is " << grid.u->refcount << endl;
  //(grid.u->refcount) = 2;  
  //grid.release(); 
  //grid = grid.clone();  
  (grid.u->refcount) = 2;  

  return 0;
}




